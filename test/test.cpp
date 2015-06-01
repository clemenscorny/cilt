
#include <algorithm>
    using std::fill;
#include <iostream>
    using std::cerr;
    using std::endl;
#include <vector>
    using std::vector;

#include <gtest/gtest.h>

#include "cilt.h"
#include "cilt.hpp"
    using cilt::Excep;
    using cilt::ShiftReg;
    using cilt::FilterTransversal;
    using cilt::FilterIIR;
    using cilt::FilterForm1;
    using cilt::FilterForm2;

static void iIRFilterGeneralTest(size_t size, FilterIIR& filter);

static void iIRFilterGeneralTest(size_t size, FilterIIR& filter) {
    const vector<float>* b_filter = filter.getNumerator();
    const vector<float>* a_filter = filter.getDenumerator();
    vector<float> a(size);
    vector<float> b(size);

    for(size_t i = 0; i < size; ++i) {
        EXPECT_EQ(a_filter->at(i), 0.f);
        EXPECT_EQ(b_filter->at(i), 0.f);
        a[i] = 2.f*(i+1.f);
        b[i] = 2.f*(size-i);
    }
    a[0] = 1.f;

    filter.setNumerator(b);
    filter.setDenumerator(a);
    b_filter = filter.getNumerator();
    a_filter = filter.getDenumerator();

    for(size_t i = 0; i < size; ++i) {
        EXPECT_EQ(a[i], a_filter->at(i));
        EXPECT_EQ(b[i], b_filter->at(i));
    }

    a[0] = 2.f;
    filter.setDenumerator(a);
    a_filter = filter.getDenumerator();

    for(size_t i = 0; i < size; ++i) {
        if(i != 0) {
            EXPECT_EQ(a[i], 2.f*a_filter->at(i));
        } else {
            EXPECT_EQ(a_filter->at(i), 1.f);
        }
        EXPECT_EQ(b[i], 2.f*b_filter->at(i));
    }
}

TEST(ShiftReg, ConstAndDestructor) {
    ShiftReg<float> buff_1;
    EXPECT_EQ(buff_1.size(), 0);
    size_t size = 5;
    buff_1 = ShiftReg<float>(size);
    EXPECT_EQ(buff_1.size(), size);
    ShiftReg<float> buff_2(buff_1);
    EXPECT_EQ(buff_2.size(), size);
}

TEST(ShiftReg, Data) {
    size_t size = 10;
    ShiftReg<float> buff(size);

    cilt_Errno errno = CILT_E_NONE;
    try {
        buff[size];
    } catch(const Excep &e) {
        errno = e.error();
    }
    EXPECT_EQ(errno, CILT_E_IND_OUT_OF_RNG);

    for(size_t i = 0; i < 2*size; ++i) {
        bool is_e_op = false;
        bool is_e_at = false;
        float val_op;
        float val_at;

        try {
            val_op = buff[i];
        } catch(const Excep &e) {
            is_e_op = true;
        }

        try {
            val_at = buff.at(i);
        } catch(const Excep &e) {
            is_e_at = true;
        }

        if(i < size) {
            EXPECT_EQ(is_e_op, false);
            EXPECT_EQ(is_e_at, false);
            EXPECT_EQ(val_op, 0.f);
            EXPECT_EQ(val_at, 0.f);
        } else {
            EXPECT_EQ(is_e_op, true);
            EXPECT_EQ(is_e_at, true);
        }
    }

    for(size_t i = 1; i < 2*size; ++i) {
        buff.add(i);
        for(size_t j = 0; j < size; ++j) {
            try {
                if(j > i) {
                    EXPECT_EQ(buff[j], 0.f);
                    EXPECT_EQ(buff.at(j), 0.f);
                } else {
                    EXPECT_EQ(buff[j], i-j);
                    EXPECT_EQ(buff.at(j), i-j);
                }
            } catch(const Excep &e) {
                cerr << e.what() << endl;
            }
        }
    }
}

TEST(ShiftReg, Copy) {
    size_t size = 5;
    ShiftReg<float> buff(size);

    for(size_t i = 5; i > 0; --i) {
        buff.add(i);
    }

    ShiftReg<float> buff_1 = buff;
    ShiftReg<float> buff_2(buff);

    EXPECT_EQ(buff.size(), buff_1.size());
    EXPECT_EQ(buff.size(), buff_2.size());
    for(size_t i = 0; i < size; ++i) {
        EXPECT_EQ(buff[i], buff_1[i]);
        EXPECT_EQ(buff_1[i], buff_1.at(i));
        EXPECT_EQ(buff[i], buff_2[i]);
    }
}

TEST(FilterTransversal, General) {
    size_t size = 5;
    FilterTransversal filter(size);
    const vector<float>* b_filter = filter.getNumerator();
    vector<float> b(size);

    for(size_t i = 0; i < size; ++i) {
        EXPECT_EQ(b_filter->at(i), 0.f);
        b[i] = i+1.f;
    }

    filter.setNumerator(b);
    b_filter = filter.getNumerator();

    for(size_t i = 0; i < size; ++i) {
        EXPECT_EQ(b[i], b_filter->at(i));
    }

    b = vector<float>(size+2, 0.f);
    filter.setCoeffs(b);
    EXPECT_EQ(filter.getOrder(), size+2);
    b_filter = filter.getNumerator();

    for(size_t i = 0; i < filter.getOrder(); ++i) {
        EXPECT_EQ(b_filter->at(i), 0.f);
    }
}

TEST(FilterTransversal, Tick) {
    size_t size = 5;
    FilterTransversal filter(size);
    vector<float> x(size);
    vector<float> b(size);

    for(size_t i = 0; i < size; ++i) {
        x[i] = i+1.f;
        b[i] = 1.f;
    }

    filter.setNumerator(b);

    for(size_t i = 0; i < 2*size; ++i) {
        float res;
        float temp = (i%size)+1;
        if(i < size) {
            res = filter.tick(x[i]);
            EXPECT_EQ(res, temp*(temp+1.f)/2.f);
        } else {
            res = filter.tick(0.f);
            EXPECT_EQ(res, size*(size+1)/2-temp*(temp+1.f)/2.f);
        }
    }
}

TEST(FilterForm1, General) {
    size_t size = 5;
    FilterForm1 filter(size);

    iIRFilterGeneralTest(size, filter);
}

TEST(FilterForm2, General) {
    size_t size = 5;
    FilterForm2 filter(size);

    iIRFilterGeneralTest(size, filter);
}
