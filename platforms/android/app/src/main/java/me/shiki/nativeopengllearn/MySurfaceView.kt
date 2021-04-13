package me.shiki.nativeopengllearn

import android.content.Context
import android.graphics.Bitmap
import android.util.AttributeSet
import android.view.SurfaceHolder
import me.shiki.nativeopengllearn.native.NativeSurfaceView
import java.nio.ByteBuffer

/**
 *
 * @author shiki
 * @date 4/10/21
 *
 */
class MySurfaceView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : NativeSurfaceView(context, attrs, defStyleAttr) {

    companion object {
        const val IMAGE_FORMAT_RGBA = 0x01
        const val IMAGE_FORMAT_NV21 = 0x02
        const val IMAGE_FORMAT_NV12 = 0x03
        const val IMAGE_FORMAT_I420 = 0x04
    }

    var onSurfaceCreatedListener: (() -> Unit)? = null

    override fun surfaceCreated(holder: SurfaceHolder) {
        super.surfaceCreated(holder)
        onSurfaceCreatedListener?.invoke()
    }

    fun switchShader(unSelIndex: Int, selIndex: Int) {
        native_SwitchShader(unSelIndex, selIndex)
    }

    fun setImageData(index: Int, format: Int, bitmap: Bitmap) {
        val bytes = bitmap.byteCount
        val buf = ByteBuffer.allocate(bytes)
        bitmap.copyPixelsToBuffer(buf)
        setImageData(index, format, bitmap.width, bitmap.height, buf.array())
    }

    fun setImageData(index: Int, format: Int, width: Int, height: Int, bytes: ByteArray) {
        native_SetImageData(index, format, width, height, bytes)
    }

    private external fun native_SwitchShader(unSelIndex: Int, selIndex: Int)

    private external fun native_SetImageData(index: Int, format: Int, width: Int, height: Int, bytes: ByteArray)
}