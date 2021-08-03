package com.superbomb.json;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

public class SBJSON {

	public static Object parse(String jsonString) {
		
		try {
			Object json = new JSONTokener(jsonString).nextValue();
			return parse(json);
			
		} catch (JSONException e) {
			e.printStackTrace();
		}
		
		return null;
	}
	
	public static Object parse(Object json) {
		
		try {		
			if( json instanceof JSONObject ) {
				// json object
				JSONObject jsonObject = (JSONObject)json;
				Map<String, Object> map = new HashMap<String, Object>();
				
				Iterator<String> keyList = jsonObject.keys();
				
				while( keyList.hasNext() ) {
					String key = keyList.next();
					Object value = null;
					
					if( jsonObject.isNull(key) == false ) {
						value = jsonObject.get(key);
					}
					
					if( value instanceof JSONObject || 
						value instanceof JSONArray ) {
						value = parse(value);
					} 
					
					map.put(key, value);
				}
				
				return map;			
						
			} else if( json instanceof JSONArray ) {
				// json array
				JSONArray jsonArray = (JSONArray)json;
				ArrayList<Object> list = new ArrayList<Object>();
				
				for( int i = 0; i < jsonArray.length(); i++ ) {
					Object value = null;
					
					if( jsonArray.isNull(i) == false ) {
						value = jsonArray.get(i);					
					}
					
					if( value instanceof JSONObject || 
						value instanceof JSONArray ) {
						value = parse(value);
					} 
					
					list.add(value);
				}
				
				return list;
			}
		    
		} catch (JSONException e) {
			e.printStackTrace();
		}
	
		return null;		
	}
	
	public static String toJSON(Object obj) {
		
		return toJSONValue(obj).toString();
	}
	
	public static Object toJSONValue(Object obj) {
		
		if( obj instanceof ArrayList ) {
			JSONArray jsonArray = new JSONArray();
			ArrayList list = (ArrayList)obj;
			
			for( Object v : list ) {
				jsonArray.put(toJSONValue(v));
			}
			
			return jsonArray;
		}
		
		if( obj instanceof Map ) {
			JSONObject jsonObject = new JSONObject();			
			Map<String, Object> map = (Map<String, Object>)obj;
			
			Iterator<String> keyList = map.keySet().iterator();
			
			while( keyList.hasNext() ) {
				String key = keyList.next();
				Object v = toJSONValue(map.get(key));
				
				try {
					jsonObject.put(key, v);
				} catch (JSONException e) {}
			}
			
			return jsonObject;
		}
		
		return obj;
	}
}
