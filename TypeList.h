#ifndef TYPELIST_H
#define TYPELIST_H

namespace TL
{
    template<typename T, typename... U>
    struct TypeList
    {
        typedef T Head;
    };
    
    struct NullType;
    struct EmptyType {};
    
    template<typename TList> struct Length;
    template<typename Head, typename... Tail>
    struct Length<TypeList<Head, Tail...> >
    {
        enum {value = sizeof...(Tail) + 1};
    };
    
    template<typename TList, int index> struct TypeAt;
    template<typename Head, typename... Tail>
    struct TypeAt<TypeList<Head, Tail...>, 0>
    {
        typedef Head type;
    };
    template<typename Head, typename... Tail, int index>
    struct TypeAt<TypeList<Head, Tail...>, index>
    {
        typedef typename TypeAt<TypeList<Tail...>, index - 1>::type type;
    };
    
    template<typename TList, int index, typename ErrorType = NullType> struct TypeAtNonStrict;
    template<typename Head, int index, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head>, index, ErrorType>
    {
        typedef ErrorType type;
    };
    template<typename Head, typename... Tail, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head, Tail...>, 0, ErrorType>
    {
        typedef Head type;
    };
    template<typename Head, typename... Tail, int index, typename ErrorType>
    struct TypeAtNonStrict<TypeList<Head, Tail...>, index, ErrorType>
    {
        typedef typename TypeAt<TypeList<Tail...>, index - 1>::type type;
    };
    
    template<typename TList, typename T> struct IndexOf;
    template<typename Head, typename T>
    struct IndexOf<TypeList<Head>, T>
    {
        enum {value = -1};
    };
    template<typename... Tail, typename T>
    struct IndexOf<TypeList<T, Tail...>, T>
    {
        enum {value = 0};
    };
    template<typename Head, typename... Tail, typename T>
    struct IndexOf<TypeList<Head, Tail...>, T>
    {
    private:
        enum {index = IndexOf<TypeList<Tail...>, T>::value};
        
    public:
        enum {value = index == -1 ? -1 : index + 1};
    };
    
    template<typename TList, typename T> struct Append;
    template<typename Head, typename T>
    struct Append<TypeList<Head>, T>
    {
        typedef TypeList<Head, T> type;
    };
    template<typename Head, typename Head2>
    struct Append<TypeList<Head>, TypeList<Head2> >
    {
        typedef TypeList<Head, Head2> type;
    };
    template<typename Head, typename Head2, typename... Tail2>
    struct Append<TypeList<Head>, TypeList<Head2, Tail2...> >
    {
        typedef TypeList<Head, Head2, Tail2...> type;
    };
    template<typename Head, typename... Tail, typename T>
    struct Append<TypeList<Head, Tail...>, T>
    {
        typedef TypeList<Head, typename Append<TypeList<Tail...>, T>::type> type;
    };
    
    template<typename TList, typename T> struct Erase;
    template<typename Head, typename T>
    struct Erase<TypeList<Head>, T>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T>
    struct Erase<TypeList<T, Tail...>, T>
    {
        typedef TypeList<Tail...> type;
    };
    template<typename Head, typename... Tail, typename T>
    struct Erase<TypeList<Head, Tail...>, T>
    {
        typedef TypeList<Head, typename Erase<TypeList<Tail...>, T>::type> type;
    };
    
    template<typename TList, typename T> struct EraseAll;
    template<typename Head, typename T>
    struct EraseAll<TypeList<Head>, T>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T>
    struct EraseAll<TypeList<T, Tail...>, T>
    {
        typedef typename EraseAll<TypeList<Tail...>, T>::type type;
    };
    template<typename Head, typename... Tail, typename T>
    struct EraseAll<TypeList<Head, Tail...>, T>
    {
        typedef TypeList<Head, typename EraseAll<TypeList<Tail...>, T>::type> type;
    };
    
    template<typename TList> struct RemoveDuplicates;
    template<typename Head>
    struct RemoveDuplicates<TypeList<Head> >
    {
        typedef TypeList<Head> type;
    };
    template<typename Head, typename... Tail>
    struct RemoveDuplicates<TypeList<Head, Tail...> >
    {
    private:
        typedef typename RemoveDuplicates<TypeList<Tail...> >::type dup;
        typedef typename Erase<dup, Head>::type nodup;
        
    public:
        typedef TypeList<Head, nodup> type;
    };
    
    template<typename TList, typename T, typename U> struct Replace;
    template<typename Head, typename T, typename U>
    struct Replace<TypeList<Head>, T, U>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T, typename U>
    struct Replace<TypeList<T, Tail...>, T, U>
    {
        typedef TypeList<U, Tail...> type;
    };
    template<typename Head, typename... Tail, typename T, typename U>
    struct Replace<TypeList<Head, Tail...>, T, U>
    {
        typedef TypeList<Head, typename Replace<TypeList<Tail...>, T, U>::type > type;
    };
    
    template<typename TList, typename T, typename U> struct ReplaceAll;
    template<typename Head, typename T, typename U>
    struct ReplaceAll<TypeList<Head>, T, U>
    {
        typedef TypeList<Head> type;
    };
    template<typename... Tail, typename T, typename U>
    struct ReplaceAll<TypeList<T, Tail...>, T, U>
    {
        typedef TypeList<U, typename ReplaceAll<TypeList<Tail...>, T, U>::type > type;
    };
    template<typename Head, typename... Tail, typename T, typename U>
    struct ReplaceAll<TypeList<Head, Tail...>, T, U>
    {
        typedef TypeList<Head, typename ReplaceAll<TypeList<Tail...>, T, U>::type > type;
    };
    
    template<typename T, typename U>
    class Conversion
    {
        typedef char Small;
        class Big {char dummy[2];};
        static Small Test(U);
        static Big Test(...);
        static T MakeT();

    public:
        enum {exists = sizeof(Test(MakeT())) == sizeof(Small)};
        enum {exists2Way = exists && Conversion<U, T>::exists};
        enum {sameType = false};
    };
    template<typename T>
    class Conversion<T, T>
    {
    public:
        enum {exists = 1};
        enum {exists2Way = 1};
        enum {sameType = 1};
    };

#define SUPERSUBCLASS(T, U) \
    (Conversion<const U*, const T*>::exists && \
    !Conversion<const T*, const void*>::sameType)

    template<bool choose, typename T, typename U>
    struct Selector
    {
        typedef T type;
    };
    template<typename T, typename U>
    struct Selector<false, T, U>
    {
        typedef U type;
    };
    
    template<typename TList, typename Base> struct MostDerived;
    template<typename Head, typename Base>
    struct MostDerived<TypeList<Head>, Base>
    {
        typedef typename Selector<SUPERSUBCLASS(Base, Head), Head, Base>::type type;
    };
    template<typename Head, typename... Tail, typename Base>
    struct MostDerived<TypeList<Head, Tail...>, Base>
    {
    private:
        typedef typename MostDerived<TypeList<Tail...>, Base>::type mostdev;
        
    public:
        typedef typename Selector<SUPERSUBCLASS(mostdev, Head), Head, mostdev>::type type;
    };
    
    template<typename TList> struct DerivedToFront;
    template<typename Head>
    struct DerivedToFront<TypeList<Head> >
    {
        typedef TypeList<Head> type;
    };
    template<typename Head, typename... Tail>
    struct DerivedToFront<TypeList<Head, Tail...> >
    {
    private:
        typedef typename MostDerived<TypeList<Tail...>, Head>::type mostdev;
        typedef typename Replace<TypeList<Tail...>, mostdev, Head>::type tail;
        
    public:
        typedef TypeList<mostdev, tail> type;
    };
}

#endif //TYPELIST_H
