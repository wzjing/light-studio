// Copyright 2000-2021 JetBrains s.r.o. and contributors. Use of this source code is governed by the Apache 2.0 license that can be found in the LICENSE file.
import androidx.compose.desktop.ui.tooling.preview.Preview
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Column
import androidx.compose.material.MaterialTheme
import androidx.compose.runtime.*
import androidx.compose.ui.ExperimentalComposeUiApi
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.input.key.*
import androidx.compose.ui.window.Window
import androidx.compose.ui.window.application

@Composable
@Preview
fun App() {
    var text by remember { mutableStateOf("Hello, World!") }

    MaterialTheme {
        Column(modifier = Modifier.background(Color(0xFFEFEFEF), shape = RectangleShape)) {
            SideMenu()
        }
    }
}

@OptIn(ExperimentalComposeUiApi::class)
fun main() = application {
    Window(onCloseRequest = ::exitApplication,
        title = "Device",
        onKeyEvent = {
            if (it.isMetaPressed &&
                it.key == Key.W &&
                it.type == KeyEventType.KeyDown) {
                exitApplication()
                true
            } else false
        }) {
        App()
    }
}
