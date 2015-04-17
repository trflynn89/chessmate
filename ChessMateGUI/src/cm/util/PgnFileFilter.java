package cm.util;

import java.io.File;

import javax.swing.filechooser.FileFilter;

/**
 * FileFilter class for selecting only PGN files.
 *
 * @author Timothy Flynn (timothy.flynn@outlook.com)
 * @version August 2, 2011
 */
public class PgnFileFilter extends FileFilter
{
	private final static String m_pgn = "pgn";
	private final static String m_PGN = "PGN";

    /**
     * Determine if a file should be displayed to the user.
     * 
     * @param f The file to test.
     */
    @Override
    public boolean accept(File f)
    {
        if(f.isDirectory())
        {
            return true;
        }

        String extension = getExtension(f);
        if(extension != null)
        {
            if(extension.equals(m_pgn) || extension.equals(m_PGN))
            {
                    return true;
            }
        }

        return false;
    }
    
    /**
     * Get the extension of a file.
     * 
     * @param f The file name.
     * @return A String representing the extension for the file.
     */
    private static String getExtension(File f)
    {
        String ext = null;
        String s = f.getName();
        int i = s.lastIndexOf('.');

        if((i>0) &&  (i<s.length()-1))
        {
            ext = s.substring(i+1).toLowerCase();
        }
        return ext;
    }

    /**
     * Return the description for the filter.
     * 
     * @return String containing the accepted file types.
     */
    @Override
    public String getDescription()
    {
        return "PGN File (pgn, PGN)";
    }
}