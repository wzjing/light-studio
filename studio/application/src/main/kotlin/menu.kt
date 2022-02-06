import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.RectangleShape
import androidx.compose.ui.unit.dp

@Composable
fun SideMenu() {
    Column(
        modifier = Modifier.fillMaxHeight()
            .width(200.dp)
            .background(color = Color(0xFFFFFFFF), shape = RectangleShape),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Menu("Keyboard")
        Menu("Mouse")
    }
}

@Composable
fun Menu(text: String) {
    Box(
        modifier = Modifier
            .padding(vertical = 12.dp)
            .fillMaxWidth(0.8f)
            .height(120.dp)
            .background(
                Color(0xFFFAFAFA),
                shape = RoundedCornerShape(12.dp)
            )
    ) {
        Text(
            modifier = Modifier.padding(10.dp).align(Alignment.Center),
            text = text
        )
    }
}