#include "ResourceManager.h"

Resource* ResourceManager::CreateResourceFromFile(const wchar_t* _filePath, Graphics* graphics)
{
    std::wstring full_path = _filePath;
    auto it = m_mapResources.find(full_path);
    if (it != m_mapResources.end())
    {
        return it->second;
    }

    Resource* raw_res = this->CreateResourceFromFileConcrete(full_path.c_str(), graphics);
    if (raw_res)
    {
        Resource* res(raw_res);
        m_mapResources[full_path] = res;
        return res;
    }

    return nullptr;
}
