#ifndef TOOLS_H
#define TOOLS_H
#include <QFile>


struct RAII_FILE_HANDLE_WRITE
{
    QFile& i_;
    explicit RAII_FILE_HANDLE_WRITE(QFile& in) : i_(in)
    {
        bool f = i_.open(QIODevice::WriteOnly | QIODevice::Append);
        if (!f)
            qWarning() << "[LOAD] can't write!";
    }

    ~RAII_FILE_HANDLE_WRITE()
    {
        i_.close();
    }
};


struct RAII_FILE_HANDLE_READ
{
    QFile& i_;
    explicit RAII_FILE_HANDLE_READ(QFile& in): i_(in)
    {
        bool f =i_.open(QFile::ReadOnly);
        if(!f)
            qWarning() << "[LOAD] can't open or json file missing! if continue, you must write your msg with json format in left editer";
    }

    ~RAII_FILE_HANDLE_READ()
    {
        i_.close();
    }
};

template<typename...> struct List{};
template<typename T, typename... U>
struct List<T, U...>
{
  using Head = T;
  using Tail = List<U...>;
};

template<unsigned int, typename> struct TypeAt;
template<> struct TypeAt<0, List<>>
{
  using type = List<>;
};
template<typename Head, typename... Tail>
struct TypeAt<0, List<Head, Tail...>>
{
  using type = Head;
};
template<unsigned int idx, typename Head, typename... Tail>
struct TypeAt<idx, List<Head, Tail...>>
{
  using type = typename TypeAt<idx - 1, List<Tail...>>::type;
};

#endif // TOOLS_H
