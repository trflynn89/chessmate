package com.flynn.chessmate;

import com.flynn.chessmate.gui.InitializeGameGUI;

import javax.swing.UIManager;
import javax.swing.UIManager.LookAndFeelInfo;

/**
 * Main architecture to run the application.
 *
 * @author Timothy Flynn (trflynn89@pm.me)
 * @version August 10, 2011
 */
public class ChessMateGUI
{
    public static void main(String args[])
    {
        boolean nimbusSet = false;

        // Search for Nimbus look and feel
        for (LookAndFeelInfo info : UIManager.getInstalledLookAndFeels())
        {
            if (info.getName().equals("Nimbus"))
            {
                // Set to Nimbus
                try
                {
                    UIManager.setLookAndFeel(info.getClassName());
                    nimbusSet = true;
                }
                catch (Exception e)
                {
                }
                break;
            }
        }

        // If unsupported, set to system default
        if (!nimbusSet)
        {
            try
            {
                UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
            }
            catch (Exception e)
            {
            }
        }

        // Start the game
        new InitializeGameGUI().createAndShowGUI();
    }
}
