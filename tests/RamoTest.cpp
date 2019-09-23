//
// Created by nestor on 22/09/19.
//

#include <vector>
#include "gtest/gtest.h"
#include "Ramo/Ramo.h"

using std::vector;

TEST(RamoFixture, Setter) {

    Ramo ramo(1, TipoFlor::Tulipan);
    EXPECT_EQ(TipoFlor::Tulipan, ramo.getTipoFlor());
}
TEST(RamoFixture, ListaDeRamos) {

    vector<Ramo> ramos;
    for (int i = 0; i <3 ; ++i) {
        Ramo ramo(i, TipoFlor::Tulipan);
        ramos.push_back(ramo);
    }
    EXPECT_EQ(ramos.at(0).get_productor(), 0);
    EXPECT_EQ(ramos.at(1).get_productor(), 1);
    EXPECT_EQ(ramos.at(2).get_productor(), 2);
}

TEST(RamosFixture, ToString) {

    Ramo ramo(1, TipoFlor::Tulipan);
    EXPECT_STREQ("[idProductor=1, TipoFlor=Tulipan]", ramo.toString().c_str());
}

TEST(RamosFixture, Serealizar) {

    Ramo ramo1(1, TipoFlor::Tulipan);
    Ramo ramo2(1, TipoFlor::Rosa);
    Ramo ramo3(2, TipoFlor::Tulipan);
    Ramo ramo4(2, TipoFlor::Rosa);
    EXPECT_STREQ(" 10", ramo1.serializar().c_str());
    EXPECT_STREQ(" 11", ramo2.serializar().c_str());
    EXPECT_STREQ(" 20", ramo3.serializar().c_str());
    EXPECT_STREQ(" 21", ramo4.serializar().c_str());
}

TEST(RamosFixture, Deserealizar) {

    Ramo ramo1(" 10");
    Ramo ramo2(" 11");
    Ramo ramo3(" 20");
    Ramo ramo4(" 21");
    EXPECT_EQ(1, ramo1.get_productor());
    EXPECT_EQ(1, ramo2.get_productor());
    EXPECT_EQ(2, ramo3.get_productor());
    EXPECT_EQ(2, ramo4.get_productor());
    EXPECT_EQ(Tulipan, ramo1.getTipoFlor());
    EXPECT_EQ(Rosa, ramo2.getTipoFlor());
    EXPECT_EQ(Tulipan, ramo3.getTipoFlor());
    EXPECT_EQ(Rosa, ramo4.getTipoFlor());
}