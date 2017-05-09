#ifndef TYPELIST_HIERARCHYGENERATOR_H
#define TYPELIST_HIERARCHYGENERATOR_H

#include "TypeList.h"

namespace TL
{

    // Using TypeLists to generate class hierarchy
    // technique: template template parameter
    
    template<typename T, template<class> class Unit>
    struct GenScatterHierarchy;
    template<typename AtomicType, template<class> class Unit>
    struct GenScatterHierarchy : public Unit<AtomicType>
    {
    };
    template<typename Head, template<class> class Unit>
    struct GenScatterHierarchy<TypeList<Head>, Unit>
    : public GenScatterHierarchy<Head, Unit>
    {
    };
    template<typename Head, typename... Tail, template<class> class Unit>
    struct GenScatterHierarchy<TypeList<Head, Tail...>, Unit>
    : public GenScatterHierarchy<Head, Unit>,
      public GenScatterHierarchy<TypeList<Tail...>, Unit>
    {
    };
    
    // utility functions
    template<typename T, typename TList, template<class> class Unit>
    Unit<T>& Field(GenScatterHierarchy<TList, Unit>& obj)
    {
        return obj;
    };
    template<typename T, typename TList, template<class> class Unit>
    const Unit<T>& Field(const GenScatterHierarchy<TList, Unit>& obj)
    {
        return obj;
    };
    
    // little utility class
    // turn ints to types
    template<int v>
    struct Int2Type
    {
        enum {value = v};
    };
    
    template<typename Head, typename... Tail, template<class> class Unit>
    Unit<Head>& FieldHelper(GenScatterHierarchy<TypeList<Head, Tail...>, Unit>& obj, Int2Type<0>)
    {
        GenScatterHierarchy<Head, Unit>& leftBase = obj;
        return leftBase;
    };
    template<int i, typename Head, typename... Tail, template<class> class Unit>
    Unit<typename TypeAt<TypeList<Head, Tail...>, i>::type>& FieldHelper(
        GenScatterHierarchy<TypeList<Head, Tail...>, Unit>& obj,
        Int2Type<i>)
    {
        GenScatterHierarchy<TypeList<Tail...>, Unit>& rightBase = obj;
        return FieldHelper(rightBase, Int2Type<i - 1>());
    };
    template<typename Head, typename... Tail, template<class> class Unit>
    const Unit<Head>& FieldHelper(const GenScatterHierarchy<TypeList<Head, Tail...>, Unit>& obj, Int2Type<0>)
    {
        GenScatterHierarchy<Head, Unit>& leftBase = obj;
        return leftBase;
    };
    template<int i, typename Head, typename... Tail, template<class> class Unit>
    const Unit<typename TypeAt<TypeList<Head, Tail...>, i>::type>& FieldHelper(
        const GenScatterHierarchy<TypeList<Head, Tail...>, Unit>& obj,
        Int2Type<i>)
    {
        GenScatterHierarchy<TypeList<Tail...>, Unit>& rightBase = obj;
        return FieldHelper(rightBase, Int2Type<i - 1>());
    };
    
    template<int i, typename TList, template<class> class Unit>
    Unit<typename TypeAt<TList, i>::type>& Field(
        GenScatterHierarchy<TList, Unit>& obj)
    {
        return FieldHelper(obj, Int2Type<i>());
    };
    template<int i, typename TList, template<class> class Unit>
    const Unit<typename TypeAt<TList, i>::type>& Field(
        const GenScatterHierarchy<TList, Unit>& obj)
    {
        return FieldHelper(obj, Int2Type<i>());
    };
}
#endif //TYPELIST_HIERARCHYGENERATOR_H
