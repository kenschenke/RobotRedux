# RobotRedux

Redux implementation in C++ intended for use in FRC robotics

## Introduction

RobotRedux is a C++ header-only implementation of the JavaScript Redux library.  It does not utilize
templates or any feature of C++ 11 or later.

## Reducers

Because RobotRedux does not use templates, it is necessary to write a small amount of code to handle
whatever kind of data type needed for the application store.  This is true for intrinsic C++ data types
as well as classes and structures.

RobotRedux handles calling reducers generically by using a marshalling class defined for the data type.
The marshalling class, derived from ReducerMarshallBase, stores a reference to the data type in question
as well as implements a virtual function CallReducer.

## Actions

RobotRedux defines a generic Action class that implements a type() function.  Each data type
needs to define a specific class to handle that data type as well as implement a virtual function
ClassName.

## Example

Following is an example of the code necessary to handle the bool data type.

### Bool.h

```C++
#include <Redux/Redux.h>

class BoolAction : public Action
{
public:
   BoolAction(int type, bool payload=false) : Action(type), m_payload(payload) { };

   bool payload() const { return m_payload; }
   std::string ClassName(void) const { return "BoolAction"; }

private:
   bool m_payload;
};

typedef bool (*BoolReducer)(bool OldState, const BoolAction &action);
void InstallReducer(Store &store, bool &StoreItem, BoolReducer reducer);
```

### Bool.cpp

```C++
#include "Bool.h"

class ReducerMarshallBool : public ReducerMarshallBase
{
public:
   ReducerMarshallBool(bool &StoreItem, BoolReducer reducer) 
      : m_Item(StoreItem), m_reducer(reducer) { };

   inline void CallReducer(const Action &action)
   { m_Item = m_reducer(m_Item, static_cast<const BoolAction &>(action)); }

private:
   bool &m_Item;
   BoolReducer m_reducer;
};

void InstallReducer(Store &store, bool &StoreItem, BoolReducer reducer)
{
   ReducerMarshallBool *marshall = new ReducerMarshallBool(StoreItem, reducer);
   store.InstallReducer(marshall);
}
```

### Example.cpp

```C++
#include "Bool.h"
#include <Redux/Redux.h>

bool HasGearReducer(bool OldState, const BoolAction &action)
{
   switch (action.type())
   {
   case 1:
      return true;

   case 2:
      return false;

   default:
      return false;
   }
}

class State : public StateBase
{
public:
   std::string toString(void) const;

public:
   bool HasGear;
};

std::string State::toString(void) const
{
   std::string str("HasGear: ");
   str += HasGear ? "true" : "false";
   return str;
}

int main()
{
   State state;
   Store store(state);

   InstallReducer(store, state.HasGear, HasGearReducer);
   store.Dispatch(BoolAction(1));
   printf("%s\n", state.toString().c_str());
   store.Dispatch(BoolAction(2));
   printf("%s\n", state.toString().c_str());

   return 0;
}
```
