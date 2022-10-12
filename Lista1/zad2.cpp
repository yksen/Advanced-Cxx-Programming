#include <iostream>
#include <memory>
#include <stdexcept>

namespace cpplab
{
    template <class T>
    class vector
    {
    public:
        typedef T value_type;
        vector()
        {
            capacity_ = 0;
            size_ = 0;
        }
        vector(std::initializer_list<T> list)
        {
            capacity_ = list.size();
            size_ = list.size();
            data_ = std::make_unique<T[]>(capacity_);
            std::copy(list.begin(), list.end(), data_.get());
        }
        ~vector()
        {
            data_.release();
        }
        T &operator[](size_t index)
        {
            if (index < 0 || index >= size_)
                throw std::out_of_range("vector index out of bounds");
            return data_[index];
        }
        T operator[](size_t index) const
        {
            if (index < 0 || index >= size_)
                throw std::out_of_range("vector index out of bounds");
            return data_[index];
        }
        friend std::ostream &operator<<(std::ostream &os, const vector<T> &v)
        {
            for (size_t i = 0; i < v.size_; ++i)
                os << v[i] << " ";
            return os;
        }

        template <typename U>
        auto operator*(vector<U> const &rhs)
        {
            size_t min_size = size_ < rhs.size() ? size_ : rhs.size();
            decltype(data_[0] * rhs[0]) result = 0;
            for (size_t i = 0; i < min_size; i++)
                result += data_[i] * rhs[i];
            return result;
        }

        void push_back(T item)
        {
            if (size_ >= capacity_)
                reallocate();
            data_[size_] = item;
            ++size_;
        }
        void pop_back()
        {
            if (size_ > 0)
                --size_;
        }
        void resize(size_t new_size, T value)
        {
            if (new_size > size_ && new_size <= capacity_)
                for (size_t i = size_; i < new_size; ++i)
                    data_[i] = value;
            else if (new_size > capacity_)
                for (size_t i = size_; i < new_size; ++i)
                    this->push_back(value);
            size_ = new_size;
        }
        size_t size() const
        {
            return size_;
        }

    private:
        void reallocate()
        {
            size_t new_capacity = (capacity_ == 0) ? 1 : 2 * capacity_;
            T *new_data = new T[new_capacity];
            for (size_t i = 0; i < size_; ++i)
                new_data[i] = data_[i];
            data_.reset(new_data);
            capacity_ = new_capacity;
        }

        size_t size_;
        size_t capacity_;
        std::unique_ptr<T[]> data_;
    };
}

int main()
{
    cpplab::vector<int> int_vector;
    cpplab::vector<std::string> string_vector;

    for (int i = 0; i < 10; ++i)
        int_vector.push_back(i);
    int_vector[5] = 999;
    std::cout << int_vector << std::endl;

    string_vector.resize(10, "Hello");
    string_vector.resize(2, "");
    string_vector.resize(5, "world");
    std::cout << string_vector << std::endl;

    cpplab::vector<int> a{1, 3, 2};
    cpplab::vector<int> b{4, 5, 6, 7};
    cpplab::vector<double> c{1.5, 1. / 3, 0.5};
    std::cout << a * b << std::endl;
    std::cout << a * c << std::endl;

    return 0;
}