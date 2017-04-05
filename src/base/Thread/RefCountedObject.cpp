#include "RefCountedObject.h"


namespace pi {


RefCountedObject::RefCountedObject(): _counter(1)
{
}


RefCountedObject::~RefCountedObject()
{
}


} // namespace pi
