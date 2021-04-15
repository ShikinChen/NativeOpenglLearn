package me.shiki.nativeopengllearn

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.util.Log
import android.view.Menu
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import com.afollestad.materialdialogs.MaterialDialog
import com.afollestad.materialdialogs.list.listItemsSingleChoice
import java.lang.Exception

class MainActivity : AppCompatActivity() {

    companion object {
        private const val TAG = "MainActivity"
    }

    private var selectedIndex = 0
    private val nsv: MySurfaceView by lazy {
        findViewById(R.id.nsv)
    }
    private val demoList by lazy {
        listOf("Triangle", "TextureMap", "NV21TextureMap", "VaoAndVbo", "FboOffscreenRendering")
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        nsv.onSurfaceCreatedListener = {
            selectedIndex = demoList.size - 1
            setDemo(selectedIndex)
        }
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.menu_main, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        val id = item.itemId
        if (id == R.id.demo) {
            MaterialDialog(this).show {
                listItemsSingleChoice(
                    items = demoList,
                    initialSelection = selectedIndex
                ) { dialog, index, text ->
                    setDemo(index)
                }
            }
        }
        return true
    }

    private fun setDemo(index: Int) {
        when (index) {
            //TextureMap
            1 -> {
                loadRGBAImage(R.drawable.test01)?.let {
                    nsv.setImageData(index, MySurfaceView.IMAGE_FORMAT_RGBA, it)
                }
            }
            2 -> {
                loadNV21Image("test02_720x510_nv21.yuv")?.let {
                    nsv.setImageData(index, MySurfaceView.IMAGE_FORMAT_NV21, 720, 510, it)
                }
            }
            4 -> {
                loadRGBAImage(R.drawable.test02)?.let {
                    nsv.setImageData(index, MySurfaceView.IMAGE_FORMAT_RGBA, it)
                }
            }
        }
        nsv.switchShader(selectedIndex, index)
        selectedIndex = index
    }

    private fun loadNV21Image(fileName: String): ByteArray? {
        try {
            val inputStream = assets.open(fileName)
            return inputStream.readBytes()
        } catch (e: Exception) {
            if (!e.message.isNullOrEmpty()) {
                Log.e(TAG, e.message!!)
            }
        }
        return null
    }

    private fun loadRGBAImage(imgResId: Int): Bitmap? {
        try {
            val inputStream = resources.openRawResource(imgResId)
            return BitmapFactory.decodeStream(inputStream)
        } catch (e: Exception) {
            if (!e.message.isNullOrEmpty()) {
                Log.e(TAG, e.message!!)
            }
        }
        return null
    }
}