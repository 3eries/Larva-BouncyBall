package com.superbomb.math;

public class Size {
    private float w;
    private float h;

    public Size() {

    }

    public Size(float w, float h) {
        this.w = w;
        this.h = h;
    }

    public Size(Size size) {
        this(size.getWidth(), size.getHeight());
    }

    public void setWidth(float w) {
        this.w = w;
    }

    public void setHeight(float h) {
        this.h = h;
    }

    public float getWidth() {
        return w;
    }

    public float getHeight() {
        return h;
    }
}
