
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
    

    struct MeshDecl
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

    struct AddMeshError
    {
        enum Enum 
        {
            Success,
            Error,
            IndexOutOfRange,
            InvalidIndexCount,
        };
    };
    

	AddMeshError::Enum AddMesh(Packer *packer, const MeshDecl &meshDecl, uint32_t meshCountHint = 0);

    void AddMeshJoin(Packer *packer);

    struct UvMeshDcal
    {
        const void *vertexPositionData = nullptr; 
        const void *vertexUvData = nullptr;
        const void *indexData = nullptr;
        const uint32_t *faceMaterialData = nullptr;
        uint32_t vetexCount = 0;
        uint32_t vertexPositionStride = 0;
        uint32_t vertexStride = 0;
        uint32_t indexCount = 0;
        int32_t indexOffset = 0;
        IndexFormat::Enum indexFormat = IndexFormat::UInt16;
        bool rotateCharts  = true;
    };

    AddMeshError::Enum AddUvMesh(Packer *packer, const UvMeshDcal &decl);

    typedef void(*ParameterizeFunc)(const float *positions, float *texcoords, uint32_t vertexCount, const uint32_t *indices, uint32_t indexCount);

         struct ChartOptions
         {
            float maxChartArea = 0.0f;
            float maxBoundaryLength = 0.0f;
            float normalDeviationweight = 2.0f;
            float roundnessWeight = 0.01f;
            float straightnessWeight = 6.0f;
            float normalSeamWeight = 0.5f;
            float maxCost = 2.0f;
            uint32_t maxIterations = 1;
            ParameterizeFunc paramFunc = nullptr;
            bool closeHoles = false;
            bool fixTJunctions = true;
         };
    
    void ComputeCharts(Packer *packer, ChartOptions options = ChartOptions());

    struct PackOptions
    {
        bool bilinear = true;
        bool blockAlign = false;
        bool bruteForce = false;
        bool createImage = false;
        
        uint32_t maxChartSize = 0;
        uint32_t padding = 0;
        float texelsPerUnit = 0.0f;
        uint32_t resolution = 0;
    };

    void PackCharts(Packer *packer, PackOptions packOptions = PackOptions());

    void Generate(Packer *packer, ChartOptions chartOptions = ChartOptions(), PackOptions packOptions = PackOptions());

    struct ProgressCategory
    {
        enum Enum
        {
            AddMesh,
            ComputeCharts,
            ParameterizeCharts,
            PackCharts,
            BuildOutputMeshes,
        };
    };
    
    typedef bool(*ProgressFunc)(ProgressCategory::Enum category, int progress, void *userData);

    void SetProgressCallBack(Packer *packer, ProgressFunc pregressFunc = nullptr, void *progressUserData = nullptr);

    typedef void *(*ReallocFunc)(void *, size_t);
    typedef void(*FreeFunc)(void *);
    void SetAlloc(ReallocFunc reallocFunc, FreeFunc freeFunc = nullptr);
    typedef int(*PrintFunc)(const char *, ...);
    void SetPrint(PrintFunc print, bool verbose);
    const char *StringForEnum(AddMeshError::Enum error);
    const char *StringForEnum(ProgressCategory::Enum category);
    
}

#endif