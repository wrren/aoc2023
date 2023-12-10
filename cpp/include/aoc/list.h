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
        typedef std::vector<T>::const_iterator  const_iterator;
        typedef std::vector<T>::iterator        iterator;

        template<typename A>
        bool reduce(std::function<bool(const T&, A&)> reducer, A& initial = {}) const
        {
            for(auto& e : m_list)
            {
                if(!reducer(e, initial))
                {
                    return false;
                }
            }

            return true;
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

        bool all(std::function<bool(const T&)> evaluator) const
        {
            for(auto& e : m_list)
            {
                if(evaluator(e) == false)
                {
                    return false;
                }
            }

            return true;
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

        const_iterator begin() const
        {
            return m_list.begin();
        }

        const_iterator end() const
        {
            return m_list.end();
        }

        iterator begin()
        {
            return m_list.begin();
        }

        iterator end()
        {
            return m_list.end();
        }

    private:

        std::vector<T> m_list;
    };
}