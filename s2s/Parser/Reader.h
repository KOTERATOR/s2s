//
// Created by iashi on 29.11.2021.
//

#ifndef S2S_READER_H
#define S2S_READER_H


template<typename T>
class Reader {
private:
    size_t currentItem = 0;
protected:
    std::vector<T> elements{};
public:
    Reader(const std::vector<T> &readerElements) : elements(readerElements)
    {

    }

    Reader() = default;

    size_t getIndex() { return currentItem-1; }
    size_t size() { return elements.size(); }

    T *get(size_t index)
    {
        if (index >= 0 && index < elements.size())
        {
            return &elements[index];
        }
        return nullptr;
    }

    T *next()
    {
        if (currentItem <= elements.size() && !elements.empty())
        {
            currentItem++;
            auto token = get(currentItem-1);
            if (token == nullptr)
                currentItem = elements.size();
            return token;
        }
        return nullptr;
    }

    T *prev()
    {
        if (currentItem > 1)
        {
            currentItem--;
            return get(currentItem-1);
        }
        return nullptr;
    }

    T *current()
    {
        return get(currentItem-1);
    }

    T *lookNext()
    {
        if (currentItem < elements.size() && !elements.empty())
        {
            return get(currentItem);
        }
        return nullptr;
    }
};


#endif //S2S_READER_H
