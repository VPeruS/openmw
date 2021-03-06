#include "terrainstorage.hpp"

#include <boost/algorithm/string.hpp>

#include "../mwbase/world.hpp"
#include "../mwbase/environment.hpp"
#include "../mwworld/esmstore.hpp"

namespace MWRender
{

    TerrainStorage::TerrainStorage(const VFS::Manager* vfs, const std::string& normalMapPattern, const std::string& normalHeightMapPattern, bool autoUseNormalMaps, const std::string& specularMapPattern, bool autoUseSpecularMaps)
        : ESMTerrain::Storage(vfs, normalMapPattern, normalHeightMapPattern, autoUseNormalMaps, specularMapPattern, autoUseSpecularMaps)
    {
    }

    void TerrainStorage::getBounds(float& minX, float& maxX, float& minY, float& maxY)
    {
        minX = 0, minY = 0, maxX = 0, maxY = 0;

        const MWWorld::ESMStore &esmStore =
            MWBase::Environment::get().getWorld()->getStore();

        MWWorld::Store<ESM::Cell>::iterator it = esmStore.get<ESM::Cell>().extBegin();
        for (; it != esmStore.get<ESM::Cell>().extEnd(); ++it)
        {
            if (it->getGridX() < minX)
                minX = static_cast<float>(it->getGridX());
            if (it->getGridX() > maxX)
                maxX = static_cast<float>(it->getGridX());
            if (it->getGridY() < minY)
                minY = static_cast<float>(it->getGridY());
            if (it->getGridY() > maxY)
                maxY = static_cast<float>(it->getGridY());
        }

        // since grid coords are at cell origin, we need to add 1 cell
        maxX += 1;
        maxY += 1;
    }

    const ESM::Land* TerrainStorage::getLand(int cellX, int cellY)
    {
        const MWWorld::ESMStore &esmStore =
            MWBase::Environment::get().getWorld()->getStore();
        ESM::Land* land = esmStore.get<ESM::Land>().search(cellX, cellY);
        if (!land)
            return NULL;

        const int flags = ESM::Land::DATA_VCLR|ESM::Land::DATA_VHGT|ESM::Land::DATA_VNML|ESM::Land::DATA_VTEX;
        if (!land->isDataLoaded(flags))
            land->loadData(flags);

        // TODO: unload land data when it's no longer needed

        return land;
    }

    const ESM::LandTexture* TerrainStorage::getLandTexture(int index, short plugin)
    {
        const MWWorld::ESMStore &esmStore =
            MWBase::Environment::get().getWorld()->getStore();
        return esmStore.get<ESM::LandTexture>().search(index, plugin);
    }

}
