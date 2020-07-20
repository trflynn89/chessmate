package com.flynn.chessmate.util;

import javax.imageio.ImageIO;
import javax.swing.JPanel;

import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.image.FilteredImageSource;
import java.awt.image.ImageFilter;
import java.awt.image.ImageProducer;
import java.awt.image.ReplicateScaleFilter;
import java.io.File;
import java.io.IOException;

/**
 * Construct a JPanel to resize and hold an image.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version March 8, 2011
 */
public class ImagePanel extends JPanel
{
    private static final long serialVersionUID = 1L;

    private Image m_img;
    private Image m_scaledImg;
    private int m_imgWidth;
    private int m_imgHeight;

    /**
     * Create a square panel with the given image resized to the preconfigured
     *  size.
     *
     * @param img The absolute path of the image.
     * @param size The size to resize the image to.
     */
    public ImagePanel(String img, int size)
    {
        this(img, size, size);
    }

    /**
     * Create an panel with the given image resized to the preconfigured size.
     *
     * @param img The absolute path of the image.
     * @param width The width to resize the image to.
     * @param height The height to resize the image to.
     */
    public ImagePanel(String img, int width, int height)
    {
        this.m_imgWidth = width;
        this.m_imgHeight = height;

        try
        {
            this.m_img = ImageIO.read(new File(img));
        }
        catch (IOException e)
        {
            e.printStackTrace();
            return;
        }

        Dimension size = new Dimension(m_imgWidth, m_imgHeight);
        setPreferredSize(size);
        setMinimumSize(size);
        setMaximumSize(size);
        setSize(size);
        setLayout(null);
        scaleImage();
    }

    /**
     * Scale the image to the correct size.
     */
    private void scaleImage()
    {
        ImageFilter replicate = new ReplicateScaleFilter(m_imgWidth, m_imgHeight);
        ImageProducer prod = new FilteredImageSource(m_img.getSource(), replicate);
        m_scaledImg = createImage(prod);
    }

    /**
     * Render the image.
     *
     * @param g The graphics object for the image.
     */
    @Override
    public void paintComponent(Graphics g)
    {
        g.drawImage(m_scaledImg, 0, 0, null);
    }
}
