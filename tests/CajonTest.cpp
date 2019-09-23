#include "gtest/gtest.h"
#include "Ramo/Ramo.h"
#include "Cajon/Cajon.h"
#include <vector>

using std::vector;

TEST(CajonFixture, DeserealizarRamos10AUnCajon) {

    int cantidadRamos = Cajon::CAPACIDAD_RAMOS;
    string mensajeSerealizado = " 00 01 10 11 01 00 00 00 00 00";
    Cajon cajon(mensajeSerealizado, cantidadRamos);

    EXPECT_EQ(cajon.ramos.at(0).get_productor(), 0);
    EXPECT_EQ(cajon.ramos.at(0).getTipoFlor(), TipoFlor::Tulipan);

    EXPECT_EQ(cajon.ramos.at(1).get_productor(), 0);
    EXPECT_EQ(cajon.ramos.at(1).getTipoFlor(), TipoFlor::Rosa);

    EXPECT_EQ(cajon.ramos.at(2).get_productor(), 1);
    EXPECT_EQ(cajon.ramos.at(2).getTipoFlor(), TipoFlor::Tulipan);

    EXPECT_EQ(cajon.ramos.at(3).get_productor(), 1);
    EXPECT_EQ(cajon.ramos.at(3).getTipoFlor(), TipoFlor::Rosa);

    EXPECT_EQ(cajon.ramos.size(), cantidadRamos);
}

TEST(CajonFixture, ContructorCopa) {

    std::vector<Ramo> ramos;

    Ramo ramo(1, Tulipan);
    Ramo ramo2(2, Rosa);
    ramos.push_back(ramo);
    ramos.push_back(ramo2);

    Cajon cajon(ramos);

    Cajon otroCajon = cajon;

    EXPECT_EQ(2, otroCajon.ramos.size());

}