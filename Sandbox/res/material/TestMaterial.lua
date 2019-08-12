material = {
    name = "Cerberus",
    shader = "res/shaders/pbr.shader",
    textures = {
        { name = "u_AlbedoTexture", path = "res/cerberus_A.png" },
        { name = "u_NormalTexture", path = "res/cerberus_N.png" },
        { name = "u_MetalnessTexture", path = "res/cerberus_M.png" },
        { name = "u_RoughnessTexture", path = "res/cerberus_R.png" },
        { name = "u_BRDFLUTTexture", path = "res/BRDF_LUT.tga" }
    },
    params = {
        { name = "u_AlbedoTexToggle", type = "bool"},
        { name = "u_AlbedoColor", type = "vec3"},
        { name = "u_NormalTexToggle", type = "bool"},
        { name = "u_MetalnessTexToggle", type = "bool"},
        { name = "u_Metalness", type = "float"},
        { name = "u_RoughnessTexToggle", type = "bool"},
        { name = "u_Roughness", type = "float"},
        {name = "u_RadiancePrefilter", type = "float"}
    },
    cubemaps = {
        { name = "u_EnvRadianceTex", path = "res/Arches_E_PineTree_Radiance.cubemap"},
        { name = "u_EnvIrradianceTex", path = "res/Arches_E_PineTree_Irradiance.cubemap"}
    }
}
