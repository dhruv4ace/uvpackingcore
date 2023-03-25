
#pragma once
#ifndef DPACKER_H
#define DPACKER_H
#include <stdint.h>


namespace dpacker {

    struct ChartType
    {
        enum Enum
        {
            Planer,
            Ortho,
            LSCM,
            Piecewise,
            Invalid,
        };
    };
    
    struct Chart
    {
        uint32_t *facearray;
        uint32_t dpackerIndex;
        uint32_t faceCount;
        ChartType::Enum type;
        uint32_t materail;
    };

    struct Vertex
    {
        int32_t dpackerIndex;
        int32_t chartIndex;
        float uv[2];
        uint32_t xref;    
    };
    
    struct Mesh
    {
        Chart *chartArray;
        uint32_t *indexArray;
        Vertex *vertexArray;
        uint32_t chartCount;
        uint32_t indexCount;
        uint32_t vertexCount;
    };

    static const uint32_t kImageChartIndexMask = 0x1FFFFFFF;
	static const uint32_t kImageHasChartIndexBit = 0x80000000;
	static const uint32_t kImageIsBilinearBit = 0x40000000;
	static const uint32_t kImageIsPaddingBit = 0x20000000;

    struct Packer
    {
        uint32_t *image;
        Mesh *meshes;
        uint32_t width;
        uint32_t height;
        uint32_t packerCount;
        uint32_t chartCount;
        uint32_t meshCount;
        float *utilization;
        float texelsPerUnit;
    };
    
    Packer *Create();

    void Destroy(Packer *packer);

    struct IndexFormat
    {
        enum Enum 
        {
            UInt16,
            UInt32
        };
    };
    

    struct MeshDec1
    {
        const void *vertexPositionData = nullptr;
        const void *vertexNormalData = nullptr;
        const void *vertexUvData = nullptr;
        const void *indexData = nullptr;

        const bool *faceIgnoreData = nullptr;

        uint32_t vertexCount = 0;
        uint32_t vertexPositionStride = 0;
        uint32_t vertexNormalStride = 0;
        uint32_t vertexUvStride = 0;
        uint32_t indexCount = 0;
        int32_t indexOffset = 0;
        IndexFormat::Enum indexFormat = IndexFormat::UInt16;

        float epsilon = 1.192092896e-07F;
    };

    
    
}