import java.io.*;

public class HtmlToC {

    private static final String PREFIX = "  client.println(\"";
    private static final String SUFFIX = "\");";

    public static void main(String[] args) throws IOException {
        String sourceFileName = args[0];
        String destinationFileName = args[1];

        try (BufferedReader br = new BufferedReader(new FileReader(sourceFileName));
             PrintWriter pw = new PrintWriter(new FileWriter(destinationFileName))) {
            String line;
            while ((line = br.readLine()) != null) {
                line = PREFIX
                        + line.replaceAll("\"", "\\\\\"")
                        .replaceAll("\\{", "\" + ")
                        .replaceAll("\\}", " + \"")
                        + SUFFIX;
                pw.println(line);
            }
        }
    }
}
