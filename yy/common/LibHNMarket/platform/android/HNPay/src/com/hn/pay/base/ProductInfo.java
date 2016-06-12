package com.hn.pay.base;

import org.json.JSONException;
import org.json.JSONObject;


public class ProductInfo {
    // ��������
	public long				number = 0;
    // ��Ʒ�۸�
	public double			price = 0;
    // ��ƷID
	public String           identifier = null;
	// ֧������ sdk
	public int				payType = -1;
	
	public String           xmlFile = null;
	
	// ������
    public String           orderID = null;
	
	public ProductInfo()
    {    
    }
	
    public ProductInfo(long number, double price, String identifier, int payType, String orderID)
    {
        // ��������
    	this.number = number;
        // ��Ʒ�۸�
    	this.price = price;
        // ��ƷID
    	this.identifier = identifier;       
    	// ֧������ sdk
    	this.payType = payType;
    	// ������
    	this.orderID = orderID;
    }

    public String serialize()
    {
    	return "";
    }
    
    public void deserialize(String data)
    {
    	try {
			JSONObject json = new JSONObject(data);
			this.number = json.getInt("number");
			this.price = json.getDouble("price");
			this.identifier = json.getString("identifier");
			this.payType = json.getInt("payType");
			this.xmlFile = json.getString("xmlFile");
			this.orderID = json.getString("orderID");
		} catch (JSONException e) {
			e.printStackTrace();
		}
    }
}
