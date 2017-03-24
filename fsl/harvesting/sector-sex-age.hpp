#pragma once

#include <fsl/harvesting/sex-age.hpp>

namespace Fsl {
namespace Harvesting {

template<class Sectors, class Sexes, class Ages>
class SectorSexAge : public Structure< SectorSexAge<Sectors, Sexes, Ages> > {
public:
    const Sectors sectors_ = Sectors::levels;

    Array<SexAge<Sexes, Ages>, Sectors> sectors;

    template<class Mirror>
    void reflect(Mirror& mirror) {
        mirror.data(sectors, "sectors");
    }

    void initialise(void) {
        for (auto sector : sectors_) sectors(sector).initialise();
    }

    template<class Population>
    void update(uint time, Population* population) {
        for (auto sector : sectors_) sectors(sector).update(time, population);  
    }
};

}
}
