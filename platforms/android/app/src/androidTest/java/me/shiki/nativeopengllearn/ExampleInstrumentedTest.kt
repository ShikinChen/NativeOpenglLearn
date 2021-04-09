package me.shiki.nativeopengllearn

import androidx.Test.platform.app.InstrumentationRegistry
import androidx.Test.ext.junit.runners.AndroidJUnit4

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*

/**
 * Instrumented Test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Test
    fun useAppContext() {
        // Context of the app under Test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("me.shiki.nativeopengllearn", appContext.packageName)
    }
}