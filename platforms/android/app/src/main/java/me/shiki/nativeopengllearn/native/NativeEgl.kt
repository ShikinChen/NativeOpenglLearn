package me.shiki.nativeopengllearn.native

import android.view.Surface

/**
 *
 * @author shiki
 * @date 4/6/21
 *
 */
class NativeEgl {
    companion object {
        init {
            System.loadLibrary("demo_android")
        }
    }

    fun create(surface: Surface) {
        native_Create(surface)
    }

    fun change(width: Int, height: Int) {
        native_Change(width, height)
    }

    fun destroy() {
        native_Destroy()
    }

    fun notifyRender() {
        native_NotifyRender()
    }

    private external fun native_Create(surface: Surface)

    private external fun native_Change(width: Int, height: Int)

    private external fun native_Destroy()

    private external fun native_NotifyRender()
}