pluginManagement {
    repositories {
        google()
        gradlePluginPortal()
        maven("https://maven.pkg.jetbrains.space/public/p/compose/dev")
    }

    plugins {
        val kotlin_version: String by settings
        val compose_version: String by settings

        kotlin("jvm") version kotlin_version apply false
        id("org.jetbrains.compose") version compose_version apply false
    }
}
rootProject.name = "studio"
include(":application")

