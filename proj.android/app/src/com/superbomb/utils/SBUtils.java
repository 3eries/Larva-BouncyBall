package com.superbomb.utils;

import android.content.res.Resources;

/**
 * Created by seongminhwang on 2018. 2. 28..
 */
public class SBUtils {

    public static int dpToPx(int dp)  {
        return (int) (dp * Resources.getSystem().getDisplayMetrics().density);
    }

    public static int pxToDp(int px) {
        return (int) (px / Resources.getSystem().getDisplayMetrics().density);
    }

    public static float dpToPx(float dp)  {
        return dp * Resources.getSystem().getDisplayMetrics().density;
    }

    public static float pxToDp(float px)  {
        return px / Resources.getSystem().getDisplayMetrics().density;
    }
}
