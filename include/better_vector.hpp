#include <vector>

template <typename T>
class BetterVector : public std::vector<T> {
public:
    using std::vector<T>::vector;

    void push_back(const T& value) {
        if (this->size() >= this->capacity()) {
            this->reserve(this->capacity() * 2);
        }
        std::vector<T>::push_back(value);
    }

    void push_back(T&& value) {
        if (this->size() >= this->capacity()) {
            this->reserve(this->capacity() * 2);
        }
        std::vector<T>::push_back(std::move(value));
    }
    
    template <typename... Args>
    void emplace_back(Args&&... args) {
        if (this->size() >= this->capacity()) {
            this->reserve(this->capacity() * 2);
        }
        std::vector<T>::emplace_back(std::forward<Args>(args)...);
    }
};