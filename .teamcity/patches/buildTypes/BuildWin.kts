package patches.buildTypes

import jetbrains.buildServer.configs.kotlin.v2019_2.*
import jetbrains.buildServer.configs.kotlin.v2019_2.ui.*

/*
This patch script was generated by TeamCity on settings change in UI.
To apply the patch, change the buildType with id = 'BuildWin'
accordingly, and delete the patch script.
*/
changeBuildType(RelativeId("BuildWin")) {
    params {
        remove {
            password("LICENSE_KEY", "credentialsJSON:3fdfbbdf-f8f0-43e6-a1d9-87d30c3c10d2", label = "License key", description = "Value to use for integrated Cinecoder license key", display = ParameterDisplay.HIDDEN)
        }
        add {
            password("LICENSE_KEY_2025", "credentialsJSON:6e2c438c-8683-4027-9c73-098432d85fce", label = "License key", description = "Value to use for integrated Cinecoder license key", display = ParameterDisplay.HIDDEN)
        }
    }
}
