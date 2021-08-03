package com.superbomb.security;

import java.security.spec.AlgorithmParameterSpec;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import android.util.Base64;

public class SBSecurity {

	private static final String CIPHER_TRANSFORMATION_KEY = "AES/CBC/PKCS5Padding";
	public static final byte[] ivBytes = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	
	/**
	 *  AES256 암호화
	 */
	public static String encryptAES256(String str, String key) {

		try {
			SecretKeySpec newKey = new SecretKeySpec(key.getBytes("UTF-8"), "AES");		
			AlgorithmParameterSpec ivSpec = new IvParameterSpec(ivBytes);
			
			Cipher cipher = Cipher.getInstance(CIPHER_TRANSFORMATION_KEY);
			cipher.init(Cipher.ENCRYPT_MODE, newKey, ivSpec);
	
			byte[] bytes = str.getBytes("UTF-8");
			
			return Base64.encodeToString(cipher.doFinal(bytes), 0);
			
		} catch(Exception e) {
			e.printStackTrace();
		}
		
		return null;
	} 

	/**
	 *  AES256 복호화
	 */	
	public static String decryptAES256(String str, String key) {

		try {
			SecretKeySpec newKey = new SecretKeySpec(key.getBytes("UTF-8"), "AES");		
			AlgorithmParameterSpec ivSpec = new IvParameterSpec(ivBytes);
			
			Cipher cipher = Cipher.getInstance(CIPHER_TRANSFORMATION_KEY);
			cipher.init(Cipher.DECRYPT_MODE, newKey, ivSpec);
			
			byte[] bytes = Base64.decode(str, 0);
			
			return new String(cipher.doFinal(bytes), "UTF-8");
			
		} catch(Exception e) {
			e.printStackTrace();
		}			
		
		return null;
	}	
}
