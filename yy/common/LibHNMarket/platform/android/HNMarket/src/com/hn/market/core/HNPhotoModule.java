//Copyright (c) 2012��2015 Beijing TianRuiDiAn Network Technology Co,Ltd. All rights reserved

package com.hn.market.core;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.IOException;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Base64;
import android.util.Log;
import com.hn.market.base.Method;
import com.hn.market.base.Module;
import com.hn.market.export.ChannelExport;

public class HNPhotoModule extends Module {
	private static final String TAG = HNPhotoModule.class.getSimpleName();
	
	private static final int TAKE_PICTURE = 0;
	private static final int CHOOSE_PICTURE = 1;
	private static final int CROP = 2;
	private static final int CROP_PICTURE = 3;
	
	private static HNPhotoModule _photoModule = null;
	
	private String _callback = "";
	private int _cropX = 500;
	private int _cropY = 500;
	public static HNPhotoModule getInstance() {
		if (_photoModule == null) {
			synchronized (HNPhotoModule.class) {
				if (_photoModule == null) {
					_photoModule = new HNPhotoModule();
				}
			}
		}
		return _photoModule;
	}

	public HNPhotoModule() {
		super("hnphoto");
		Register("getphoto",new GetPhoto());
		Log.d(TAG, "init hnphoto");
	}
	
	//��ȡͼƬ
	private void cropImage(Uri uri, int outputX, int outputY, int requestCode) {
			Intent intent = new Intent("com.android.camera.action.CROP");  
	        intent.setDataAndType(uri, "image/*");  
	        intent.putExtra("crop", "true");  
	        intent.putExtra("aspectX", outputX);  
	        intent.putExtra("aspectY", outputY);  
	        intent.putExtra("outputX", outputX);   
	        intent.putExtra("outputY", outputY); 
	        intent.putExtra("outputFormat", "JPEG");
	        intent.putExtra("noFaceDetection", true);
	        intent.putExtra("return-data", true);  
		    activity.startActivityForResult(intent, requestCode);
	}
	
	//Bitmap����base64����
	private String bitmapToBase64(Bitmap bitmap) {  
		  
	    String result = null;  
	    ByteArrayOutputStream baos = null;  
	    try {  
	        if (bitmap != null) {  
	            baos = new ByteArrayOutputStream();  
	            bitmap.compress(Bitmap.CompressFormat.JPEG, 100, baos);  
	  
	            baos.flush();  
	            baos.close();  
	  
	            byte[] bitmapBytes = baos.toByteArray();  
	            result = Base64.encodeToString(bitmapBytes, Base64.DEFAULT);  
	        }  
	    } catch (IOException e) {  
	        e.printStackTrace();  
	    } finally {  
	        try {  
	            if (baos != null) {  
	                baos.flush();  
	                baos.close();  
	            }  
	        } catch (IOException e) {  
	            e.printStackTrace();  
	        }  
	    }  
	    return result;  
	}  
	
	private Bitmap compressImage(Bitmap image)
	{
		ByteArrayOutputStream baos = new ByteArrayOutputStream();  
        image.compress(Bitmap.CompressFormat.JPEG, 100, baos);//����ѹ������������100��ʾ��ѹ������ѹ��������ݴ�ŵ�baos��
        int options = 100;
        while ( baos.toByteArray().length / 1024 > 100) {  //ѭ���ж����ѹ����ͼƬ�Ƿ����100kb,���ڼ���ѹ��        
       	    baos.reset();//����baos�����baos  
            image.compress(Bitmap.CompressFormat.JPEG, options, baos);//����ѹ��options%����ѹ��������ݴ�ŵ�baos��
            options -=10;//ÿ�ζ�����10��ע���п���Ϊ����Ϊ���ᱨ��  
            if(options < 10) break;
        }  
        ByteArrayInputStream isBm = new ByteArrayInputStream(baos.toByteArray());//��ѹ���������baos��ŵ�ByteArrayInputStream��  
        Bitmap bitmap = BitmapFactory.decodeStream(isBm, null, null);//��ByteArrayInputStream��������ͼƬ
        return bitmap;
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (resultCode == Activity.RESULT_OK) {
			switch (requestCode) {
			case TAKE_PICTURE:
				/*
				//�������ڱ��ص�ͼƬȡ������С����ʾ�ڽ�����
				Bitmap bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory()+"/image.jpg");
				Bitmap newBitmap = ImageTools.zoomBitmap(bitmap, bitmap.getWidth() / SCALE, bitmap.getHeight() / SCALE);
				//����Bitmap�ڴ�ռ�ýϴ�������Ҫ�����ڴ棬����ᱨout of memory�쳣
				bitmap.recycle();
				
				//���������ͼƬ��ʾ�ڽ����ϣ������浽����
				iv_image.setImageBitmap(newBitmap);
				ImageTools.savePhotoToSDCard(newBitmap, Environment.getExternalStorageDirectory().getAbsolutePath(), String.valueOf(System.currentTimeMillis()));
				*/
				break;
			case CHOOSE_PICTURE:
				/*
				ContentResolver resolver = activity.getContentResolver();
				//��Ƭ��ԭʼ��Դ��ַ
				Uri originalUri = data.getData(); 
	            try {
	            	//ʹ��ContentProviderͨ��URI��ȡԭʼͼƬ
					Bitmap photo = MediaStore.Images.Media.getBitmap(resolver, originalUri);
					if (photo != null) {
						//Ϊ��ֹԭʼͼƬ�������ڴ��������������Сԭͼ��ʾ��Ȼ���ͷ�ԭʼBitmapռ�õ��ڴ�
						Bitmap smallBitmap = ImageTools.zoomBitmap(photo, photo.getWidth() / 100, photo.getHeight() / 100);
						//�ͷ�ԭʼͼƬռ�õ��ڴ棬��ֹout of memory�쳣����
						photo.recycle();
						
						String base64 = bitmapToBase64(smallBitmap);
						if(_callback != "")
						{
							ChannelExport.getInstance().executeAyncMethod(_callback,base64);
						}
					}
				} catch (FileNotFoundException e) {
				    e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				}
				break;
				*/
			case CROP:
			{
				Uri uri = null;
				if (data != null) {
					uri = data.getData();
				}else {
					String fileName = activity.getSharedPreferences("temp",Context.MODE_WORLD_WRITEABLE).getString("tempName", "");
					uri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),fileName));
				}
				cropImage(uri, _cropX, _cropY, CROP_PICTURE);
			}
				break;
			
			case CROP_PICTURE:
			{
				Bitmap photo = null;
				Uri photoUri = data.getData();
				if (photoUri != null) {
					photo = BitmapFactory.decodeFile(photoUri.getPath());
				}
				if (photo == null) {
					Bundle extra = data.getExtras();
					if (extra != null) {
		                photo = (Bitmap)extra.get("data");  
		            }  
				}
				Bitmap smallPhoto = null;
				if(photo != null) {//ѹ��
					smallPhoto = compressImage(photo);
				}
				String base64 = bitmapToBase64(smallPhoto);
				if(_callback != "") {
					ChannelExport.getInstance().executeAyncMethod(_callback,base64);
				}
			}
				break;
			default:
				break;
			}
		}
	}
	
	public class GetPhoto implements Method {

		@Override
		public String Execute(String args, String callBack) {
			JSONObject json;
			try {
				json = new JSONObject(args);
				_cropX = json.getInt("x");
				_cropY = json.getInt("y");
			} catch (JSONException e) {
				e.printStackTrace();
			}
			_callback = callBack;
			activity.runOnUiThread(new Runnable(){
				@Override
				public void run() {
					
					AlertDialog.Builder builder = new AlertDialog.Builder(activity);
					builder.setTitle("��ѡ��ͼƬ��Դ");
					builder.setNegativeButton("ȡ��", null);
					builder.setItems(new String[]{"����","���"}, new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							switch (which) {
							case TAKE_PICTURE:
							{
								Uri imageUri = null;
								String fileName = null;
								Intent openCameraIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
								//ɾ����һ�ν�ͼ����ʱ�ļ�
								SharedPreferences sharedPreferences = activity.getSharedPreferences("temp",Context.MODE_WORLD_WRITEABLE);
								ImageTools.deletePhotoAtPathAndName(Environment.getExternalStorageDirectory().getAbsolutePath(), sharedPreferences.getString("tempName", ""));
								//���汾�ν�ͼ��ʱ�ļ�����
								fileName = String.valueOf(System.currentTimeMillis()) + ".jpg";
								Editor editor = sharedPreferences.edit();
								editor.putString("tempName", fileName);
								editor.commit();
								imageUri = Uri.fromFile(new File(Environment.getExternalStorageDirectory(),fileName));
								//ָ����Ƭ����·����SD������image.jpgΪһ����ʱ�ļ���ÿ�����պ����ͼƬ���ᱻ�滻
								openCameraIntent.putExtra(MediaStore.EXTRA_OUTPUT, imageUri);
								activity.startActivityForResult(openCameraIntent, CROP);
							}
							break;
								
							case CHOOSE_PICTURE:
							{
								Intent openAlbumIntent = new Intent(Intent.ACTION_GET_CONTENT);
								openAlbumIntent.setDataAndType(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, "image/*");
								activity.startActivityForResult(openAlbumIntent, CROP);
							}
							break;

							default:
								break;
							}
						}
					});
					builder.create().show();
				}
				});
			return "";
		}
	}
}

