#include <RegistroVenta/RegistroVenta.h>
#include "gtest/gtest.h"


TEST(RegistroVentaFixture, SinVentas) {

    //given
    RegistroVenta registroVenta;
    //when
    Informe informe = registroVenta.generarInforme();
    //then
    EXPECT_EQ(TipoFlor::Ninguno, informe.getFlorMasComprada());

}

TEST(RegistroVentaFixture, TulipanMasVendido_Y_Productor_4_MasVendio) {

    //given
    RegistroVenta registroVenta;
    //when
    Ramo ramo(4, Tulipan);
    registroVenta.contabilizarRamoVendido(ramo);
    Informe informe = registroVenta.generarInforme();
    //then
    EXPECT_EQ(TipoFlor::Tulipan, informe.getFlorMasComprada());
    EXPECT_EQ(4, informe.getProductorMejorVenta());

}