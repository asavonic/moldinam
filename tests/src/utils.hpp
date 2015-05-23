#pragma once

template <class T1, class T2>
void EXPECT_CONTAINERS_EQUAL(const T1& lhs, const T2& rhs)
{
    ASSERT_EQ(lhs.size(), rhs.size());

    for (size_t i = 0; i < lhs.size(); i++) {
        EXPECT_EQ(lhs[i], rhs[i]);
    }
}


