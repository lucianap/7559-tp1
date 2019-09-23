#include "gtest/gtest.h"
/*#include "Distribuidor/Distribuidor.h"
#include "Logger/LoggerProcess.h"
#include "Ramo/Ramo.h"
#include "Cajon/Cajon.h"

TEST(DitribuidorFixture, ClasificarRosasParaEnvioAVenta) {

    Logger logger(true);
    Distribuidor distribuidor(logger);

    vector<Ramo> ramos;
    for (int i = 0; i <3 ; ++i) {
        Ramo ramo(i, TipoFlor::Tulipan);
        ramos.push_back(ramo);
    }

    Cajon cajon(ramos);

    distribuidor.clasificar(cajon);

    EXPECT_EQ(3, distribuidor.getStockTulipanes().size());
    EXPECT_EQ(0, distribuidor.getStockRosas().size());
    EXPECT_FALSE(distribuidor.hayDiponiblidadParaEnvio());

}
*/