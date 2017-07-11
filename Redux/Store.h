/**
 * Copyright 2017 Ken Schenke
 *
 * Permission is hereby granted, free of charge, to any person obtaining a 
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 * and/or sell copies of the Software, and to permit persons to whom the 
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included 
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef STORE_H
#define STORE_H

#include "MarshallBase.h"
#include "MiddleWare.h"
#include "State.h"

#include <vector>

class Store
{
public:
	Store (StateBase &state) : m_state(state) { };
	inline void InstallReducer(ReducerMarshallBase *reducer) { m_reducers.push_back(reducer); }
	inline const StateBase & getState(void) const { return m_state; }

	inline void ApplyMiddleware(Middleware * middleware) { m_middleware.push_back(middleware); }

	inline void Dispatch(const Action &action)
	{
		std::vector< Middleware * >::iterator mit;
		std::vector< Middleware * >::reverse_iterator rmit;
		std::vector< ReducerMarshallBase * >::const_iterator rit;

		for (mit = m_middleware.begin(); mit != m_middleware.end(); ++mit)
		{
			(*mit)->before(*this, action);
		}

		for (rit = m_reducers.begin(); rit != m_reducers.end(); ++rit)
		{
			(*rit)->CallReducer(action);
		}

		for (rmit = m_middleware.rbegin(); rmit != m_middleware.rend(); ++rmit)
		{
			(*rmit)->after(*this, action);
		}
	}

private:
	std::vector< ReducerMarshallBase * > m_reducers;
	std::vector< Middleware * > m_middleware;
	StateBase &m_state;
};

#endif // end of STORE_H
