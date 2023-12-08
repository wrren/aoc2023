#include <vector>

namespace aoc
{
    template<typename T>
    class list
    {
    public:

        typedef std::vector<T>::size_type       size_type;
        typedef std::vector<T>::reference       reference;
        typedef std::vector<T>::const_reference const_reference;

        template<typename A>
        A reduce(std::function<A(const T&, const A&)> reducer, const A& initial = {}) const
        {
            A result = initial;

            for(auto& e : m_list)
            {
                result = reducer(e, result);
            }

            return result;
        }

        template<typename M>
        list<M> map(std::function<M(const T&)> mapper) const
        {
            list<M> result;
            result.reserve(size());

            for(auto& e : m_list)
            {
                result.push_back(mapper(e));
            }

            return result;
        }

        void reserve(size_type size)
        {
            m_list.reserve(size);
        }

        void resize(size_type size)
        {
            m_list.resize(size);
        }

        size_type size() const
        {
            return m_list.size();
        }

        void push_back(const T& in)
        {
            m_list.push_back(in);
        }

        reference operator[](size_type i)
        {
            return m_list[i];
        }

        const_reference operator[](size_type i) const
        {
            return m_list[i];
        }

    private:

        std::vector<T> m_list;
    };
}