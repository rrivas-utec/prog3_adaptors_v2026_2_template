#ifndef PROG3_ADAPTADORES_2026_2_ADAPTOR_CONCEPTS_H
#define PROG3_ADAPTADORES_2026_2_ADAPTOR_CONCEPTS_H

#include <concepts>

template <typename Container>
concept SequenceContainerConcept = requires(
    Container cnt, const Container const_cnt,
    const typename Container::value_type& value)
{
    cnt.push_back(value);
    cnt.front();
    cnt.back();
    { const_cnt.empty() } -> std::convertible_to<bool>;
    const_cnt.size();
};

template <typename Container>
concept PopFrontConcept = requires(Container cnt)
{
    cnt.pop_front();
};

template <typename Container>
concept PopBackConcept = requires(Container cnt)
{
    cnt.pop_back();
};

template <typename Container>
concept QueueConcept =
    SequenceContainerConcept<Container> && PopFrontConcept<Container>;

template <typename Container>
concept StackConcept =
    SequenceContainerConcept<Container> && PopBackConcept<Container>;

#endif // PROG3_ADAPTADORES_2026_2_ADAPTOR_CONCEPTS_H
