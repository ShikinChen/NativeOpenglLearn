package me.shiki.nativeopengllearn.native

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

/**
 *
 * @author shiki
 * @date 4/6/21
 *
 */
class NativeSurfaceView @JvmOverloads constructor(
    context: Context,
    attrs: AttributeSet? = null,
    defStyleAttr: Int = 0
) : SurfaceView(context, attrs, defStyleAttr), SurfaceHolder.Callback {

    private val nativeEgl by lazy {
        NativeEgl()
    }

    init {
        holder?.addCallback(this)
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        nativeEgl.native_Create(holder.surface)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        nativeEgl.native_Change(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        nativeEgl.native_Destroy()
    }
}