package com.eternal.xdsdk

/**
 * SuperJNI - Main JNI interface for plugin management and licensing
 * All methods are static and called through the Companion object
 */
class SuperJNI {
    
    companion object {
        init {
            System.loadLibrary("client")
        }
        
        /**
         * Check license validity
         * @param key License key to verify
         * @return true if license is valid
         */
        @JvmStatic
        external fun check(key: String): Boolean
        
        /**
         * Get current game version
         * @return Version string of the detected game
         */
        @JvmStatic
        external fun currGameVer(): String
        
        /**
         * Get current plugin URL
         * @return URL for plugin updates
         */
        @JvmStatic
        external fun currPlugUrl(): String
        
        /**
         * Get current plugin version
         * @return Version string of the active plugin
         */
        @JvmStatic
        external fun currPlugVer(): String
        
        /**
         * Get server time
         * @return Current server timestamp in milliseconds
         */
        @JvmStatic
        external fun getTime(): Long
        
        /**
         * Validate license
         * @param key License key to validate
         * @return true if license is valid and not expired
         */
        @JvmStatic
        external fun licence(key: String): Boolean
        
        /**
         * Update plugin from URL
         * @param url URL to download plugin update
         * @return true if update was successful
         */
        @JvmStatic
        external fun update(url: String): Boolean
        
        /**
         * Get Telegram support URL
         * @return Telegram group/channel URL for support
         */
        @JvmStatic
        external fun urlTg(): String
    }
}
