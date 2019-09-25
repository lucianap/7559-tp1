#include <RegistroVenta/RegistroVenta.h>
#include "gtest/gtest.h"


TEST(RegistroVentaFixture, SinVentas) {

    //given
    RegistroVenta registroVenta;
    //when
    Informe informe = registroVenta.generarInformes();
    //then
    EXPECT_EQ(TipoFlor::Ninguno, informe.getFlorMasComprada());

}

TEST(RegistroVentaFixture, TulipanMasVendido) {

    //given
    RegistroVenta registroVenta;
    //when
    Ramo ramo(4, Tulipan);
    Ramo ramo2(5, Tulipan);
    Ramo ramo3(4, Rosa);
    registroVenta.contabilizarRamoVendido(ramo);
    registroVenta.contabilizarRamoVendido(ramo2);
    registroVenta.contabilizarRamoVendido(ramo3);
    Informe informe = registroVenta.generarInformes();
    //then
    EXPECT_EQ(TipoFlor::Tulipan, informe.getFlorMasComprada());

}
TEST(RegistroVentaFixture, El_Productor_4_Fue_el_que_MasVendio) {

    //given
    RegistroVenta registroVenta;
    //when
    Ramo ramo(4, Tulipan);
    Ramo ramo2(5, Tulipan);
    Ramo ramo3(4, Rosa);
    registroVenta.contabilizarRamoVendido(ramo);
    registroVenta.contabilizarRamoVendido(ramo2);
    registroVenta.contabilizarRamoVendido(ramo3);
    Informe informe = registroVenta.generarInformes();
    //then
    EXPECT_EQ(4, informe.getProductorMejorVenta());

}