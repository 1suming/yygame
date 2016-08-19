package com.hn.pay.sdk;

import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Locale;
import java.util.Random;

import android.app.Activity;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.view.View;
import android.widget.Toast;

import com.alipay.sdk.app.PayTask;
import com.hn.pay.base.PayResultCallBack;
import com.hn.pay.base.PayResultCallBack.PAY_SDK;

public class AlipayManager {
	public static final String PARTNER = "2088421471797022";
	public static final String SELLER = "2088421471797022";
	public static final String RSA_PRIVATE = "MIICWwIBAAKBgQCm9QQTT/GyzK85xZleJ4Cw3wSzx5mbNmlgTzroaouY2C4ov8+v"+
		"7rNFOa0TkIa0bIYtFj4/eAwwQeNb8DUwqtLCPi6p7kUD4/nEcH3UYrwyPxwgch5X"+
		"TcKpANy7Xv9mHLrrm8eGWWbInrMcVfW8a+DAz8hCx8VWdIEXCrD314w9xQIDAQAB"+
		"AoGAE2bl0li3/c9Ljaso54gvrF4+baYEc8Ue+dI7RkcbceetdY7U090peCH7uwpQ"+
		"nSb/cL3k/fs1b2Y4xbZkeANh+oCveaXJiSsUUmbXL/6qDHbGnLSdck2wzsUe3Gk+"+
		"yR/kbPlm7hXdGXMoyyH/g+X5nMZJLQL8VCwI6gkr7WPsPQECQQDTVeUR0smmpKSr"+
		"SZni8JbWP2oF93hc3Z1erKTnxt9kUZFETGnkfCJj+awsN/RtOrMAIm1g49zq3+fI"+
		"m8j4zuq3AkEAyj4Rl3+1Pn4VLTfDcgMqcNNz5NGp3Fpoee5sPl+1BFNoZd7kobeU"+
		"eQkXmT+7+m4HXFr3acsvCbpCAvU7Pq9PYwJAaD9Op+QapKFCuVutFU0omrbdTeQs"+
		"82I+Zp0zG0H2oD28Mrye3558OGTIkWdooxj5g294FPtR1mpso4B1OoaXMQJAbY2z"+
		"39pZJtILzKPRZrhaTCXvOLtuhSQgRjSmRB96Ii7AdbaJKSUV836+XtCMlnr118yb"+
		"C576Cw3vZkTLZAh5OwJAcLIiq+4GJRVG93tu+DJzR41A910wjz/lyS2F3vYGTNXC"+
		"4lAM35Fi2WfcI2nYe9Kb5l96fqYNhckTI9N8HBUwlQ==";
	
	public static final String NOTIFY_URL = "http://www.dzpk886.com/manage/pay/alipay/alipay_mobile.aspx";

	private static final int SDK_PAY_FLAG = 1;

	private static final int SDK_CHECK_FLAG = 2;

	private Activity activity;

	private PayResultCallBack callback;

	private Handler mHandler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SDK_PAY_FLAG: {
				Result resultObj = new Result((String) msg.obj);
				String resultStatus = resultObj.resultStatus;

				// �ж�resultStatus Ϊ��9000�������֧���ɹ�������״̬�������ɲο��ӿ��ĵ�
				if (TextUtils.equals(resultStatus, "9000")) {
					callback.onPayResult(PAY_SDK.ALIPAY, 1);
				} else {
					// �ж�resultStatus Ϊ�ǡ�9000����������֧��ʧ��
					// ��8000��
					// ����֧�������Ϊ֧������ԭ�����ϵͳԭ���ڵȴ�֧�����ȷ�ϣ����ս����Ƿ�ɹ��Է�����첽֪ͨΪ׼��С����״̬��
					if (TextUtils.equals(resultStatus, "8000")) {
						Toast.makeText(activity, "֧�����ȷ����", Toast.LENGTH_SHORT)
								.show();

					} else {
						Toast.makeText(activity, "֧��ʧ��", Toast.LENGTH_SHORT)
								.show();
						callback.onPayResult(PAY_SDK.ALIPAY, 0);
					}
				}
				break;
			}
			case SDK_CHECK_FLAG: {
				Toast.makeText(activity, "�����Ϊ��" + msg.obj, Toast.LENGTH_SHORT)
						.show();
				break;
			}
			default:
				break;
			}
		};
	};

	public AlipayManager(Activity activity) {
		this.activity = activity;
	}

	/**
	 * call alipay sdk pay. ����SDK֧��
	 * 
	 */
	public void pay(String product, String productDescription,
			String productPrice,  String orderID, PayResultCallBack callback) {
		this.callback = callback;
		String orderInfo = getOrderInfo(product, productDescription, orderID,
				 productPrice);
		String sign = sign(orderInfo);
		try {
			// �����sign ��URL����
			sign = URLEncoder.encode(sign, "UTF-8");
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		}
		final String payInfo = orderInfo + "&sign=\"" + sign + "\"&"
				+ getSignType();

		Runnable payRunnable = new Runnable() {

			@Override
			public void run() {
				// ����PayTask ����
				PayTask alipay = new PayTask(activity);
				// ����֧���ӿ�
				String result = alipay.pay(payInfo);

				Message msg = new Message();
				msg.what = SDK_PAY_FLAG;
				msg.obj = result;
				mHandler.sendMessage(msg);
			}
		};

		Thread payThread = new Thread(payRunnable);
		payThread.start();
	}

	/**
	 * check whether the device has authentication alipay account.
	 * ��ѯ�ն��豸�Ƿ����֧������֤�˻�
	 * 
	 */
	public void check() {
		Runnable checkRunnable = new Runnable() {

			@Override
			public void run() {
				PayTask payTask = new PayTask(activity);
				boolean isExist = payTask.checkAccountIfExist();

				Message msg = new Message();
				msg.what = SDK_CHECK_FLAG;
				msg.obj = isExist;
				mHandler.sendMessage(msg);
			}
		};

		Thread checkThread = new Thread(checkRunnable);
		checkThread.start();

	}

	/**
	 * get the sdk version. ��ȡSDK�汾��
	 * 
	 */
	public void getSDKVersion() {
		PayTask payTask = new PayTask(activity);
		String version = payTask.getVersion();
		Toast.makeText(activity, version, Toast.LENGTH_SHORT).show();
	}

	/**
	 * create the order info. ����������Ϣ
	 * 
	 */
	public String getOrderInfo(String subject, String body, String price, String orderID) {
		
		// �������룬 �̶�ֵ
		String orderInfo = "_input_charset=\"utf-8\"";
		
		// ��Ʒ����
		orderInfo += "&body=" + "\"" + body + "\"";
		
		// ����δ����׵ĳ�ʱʱ��
				// Ĭ��30���ӣ�һ����ʱ���ñʽ��׾ͻ��Զ����رա�
				// ȡֵ��Χ��1m��15d��
				// m-���ӣ�h-Сʱ��d-�죬1c-���죨���۽��׺�ʱ����������0��رգ���
				// �ò�����ֵ������С���㣬��1.5h����ת��Ϊ90m��
		orderInfo += "&it_b_pay=\"30m\"";
		
		// �������첽֪ͨҳ��·��
		orderInfo += "&notify_url=" + "\"" + NOTIFY_URL + "\"";
		
		// �̻���վΨһ������
		orderInfo += "&out_trade_no=" + "\"" + orderID + "\"";
				
		// ���������ID
		orderInfo += "&partner=" + "\"" + PARTNER + "\"";
		
		// ֧�����ͣ� �̶�ֵ
		orderInfo += "&payment_type=\"1\"";

		// ����֧�����˺�
		orderInfo += "&seller_id=" + "\"" + SELLER + "\"";
		
		// �ӿ����ƣ� �̶�ֵ
		orderInfo += "&service=\"mobile.securitypay.pay\"";

		// ��Ʒ����
		orderInfo += "&subject=" + "\"" + subject + "\"";

		// ��Ʒ���
		orderInfo += "&total_fee=" + "\"" + price + "\"";
		
		// ֧��������������󣬵�ǰҳ����ת���̻�ָ��ҳ���·�����ɿ�
		// orderInfo += "&return_url=\"m.alipay.com\"";

		// �������п�֧���������ô˲���������ǩ���� �̶�ֵ
		// orderInfo += "&paymethod=\"expressGateway\"";

		return orderInfo;
	}

	/**
	 * get the out_trade_no for an order. ��ȡ�ⲿ������
	 * 
	 */
	public String getOutTradeNo() {
		SimpleDateFormat format = new SimpleDateFormat("MMddHHmmss",
				Locale.getDefault());
		Date date = new Date();
		String key = format.format(date);

		Random r = new Random();
		key = key + r.nextInt();
		key = key.substring(0, 15);
		return key;
	}

	/**
	 * sign the order info. �Զ�����Ϣ����ǩ��
	 * 
	 * @param content
	 *            ��ǩ��������Ϣ
	 */
	public String sign(String content) {
		return SignUtils.sign(content, RSA_PRIVATE);
	}

	/**
	 * get the sign type we use. ��ȡǩ����ʽ
	 * 
	 */
	public String getSignType() {
		return "sign_type=\"RSA\"";
	}

}
