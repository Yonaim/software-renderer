#include "asset.h"
#include <iostream>

using namespace asset;

void test_findParser()
{
    std::string str1("test1.obj");

    auto output = findParser(str1);
    std::cout << (void *)output << std::endl;
}

void test_loadAsset()
{
    auto output = loadAsset("./assets/triangle.obj");
    if (std::get_if<core::Mesh>(&output) != nullptr)
    {
        std::cout << "mesh" << std::endl;
    }
}

void test_loadScene() {}

int main()
{
    test_loadAsset();
    return 0;
}