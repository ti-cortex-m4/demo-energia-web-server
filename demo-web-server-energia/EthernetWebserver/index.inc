  client.println("<html>");
  client.println("<head><title>Energia Ethernet Web Server</title></head>");
  client.println("<body align=center>");
  client.println("<h1 align=center>Welcome To Ethernet Web Server</h1>");
  client.println("</br>");
  client.println("<table border=\"0\" align=center width=1200px height=590px>");
  client.println("    <tr>");
  client.println("        <td align=center width=375></td>");
  client.println("        <td width=450px align=left valign=\"top\">");
  client.println("            <p>The Ethernet Web Server provides the");
  client.println("                capability to remotely read and write GPIOs");
  client.println("                on/off.</p></br></br>");
  client.println("            <p><a href=\"/config.html\">Click here</a>");
  client.println("                to check status and configure the board</p>");
  client.println("        <td align=cneter width=375></td>");
  client.println("    </tr>");
  client.println("</table>");
  client.println("</body>");
  client.println("</html>");