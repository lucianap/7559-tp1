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

TEST(RegistroVentaFixture, Serealizar) {

    //given
    RegistroVenta registroVenta;
    //when
    Ramo ramo(4, Tulipan);
    Ramo ramo2(5, Tulipan);
    Ramo ramo3(4, Rosa);
    registroVenta.contabilizarRamoVendido(ramo);
    registroVenta.contabilizarRamoVendido(ramo2);
    registroVenta.contabilizarRamoVendido(ramo3);

    //then
    // cantidadFlores(2),[ tipo(2), acumulado(10)], cantidadProductores(2), [id(2), acumulado(10)]
    string serializadoEsperado= " 2 0         2 1         1 2 4         2 5         1";
    EXPECT_EQ(serializadoEsperado, registroVenta.serializar());

}


TEST(RegistroVentaFixture, SerealizarNada) {

    //given
    RegistroVenta registroVenta;
    //when : Nada

    //then
    // cantidadFlores(2),[ tipo(2), acumulado(10)], cantidadProductores(2), [id(2), acumulado(10)]
    string serializadoEsperado= " 0 0";
    EXPECT_EQ(serializadoEsperado, registroVenta.serializar());

}

TEST(RegistroVentaFixture, DeSerealizarInfoExistente) {

    //given
    // cantidadFlores(2),[ tipo(2), acumulado(10)], cantidadProductores(2), [id(2), acumulado(10)]
    string serializado= " 2 0         2 1         1 2 4         2 5         1";

    //when
    RegistroVenta registroVenta(serializado);

    //then
    Informe informe = registroVenta.generarInformes();
    EXPECT_EQ(4, informe.getProductorMejorVenta());
    EXPECT_EQ(TipoFlor::Tulipan, informe.getFlorMasComprada());


}

