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

    external fun native_Create(surface: Surface)

    external fun native_Change(width: Int, height: Int)

    external fun native_Destroy()
}