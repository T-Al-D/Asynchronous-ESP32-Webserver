#include <HTML.h>

String DesignButton(bool boolean, String unit)
{
    String result = "";
    if (unit == "button1")
    {
        if (boolean)
        {
            result =  R"rawliteral(
            <p>BUTTON 1 Status: ON</p>
            <a class="button button-on" href="/button1off">Turn off!</a>
            )rawliteral";
        }
        else
        {
            result =  R"rawliteral(
            <p>BUTTON 1 Status: OFF</p>
            <a class="button button-off" href="/button1on">Turn on!</a>
            )rawliteral";
        }
    }
    if (unit == "button2")
    {
        if (boolean)
        {
            result =  R"rawliteral(
            <p>BUTTON 2 Status: ON</p>
            <a class="button button-on" href="/button2off">Turn off!</a>
            )rawliteral";
        }
        else
        {
            result =  R"rawliteral(
            <p>BUTTON 2 Status: OFF</p>
            <a class="button button-off" href="/button2on">Turn on!</a>
            )rawliteral";
        }
    }
    return result;
}

// dynamic HTML Page
String SendHTML(bool button1status, bool button2status, String otherInfo)
{
    /* HTML text build in raw text*/
    // rawliteral = no escaped symbols
    String ptr = R"rawliteral(
    <!DOCTYPE html> <html>
    <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title> Button Control </title>

    <style>
    html { font-family: Helvetica; display: block; margin: 0px auto; text-align: center;}
    <body>
    <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
    <title>Button Control</title> 
    <style>

    html { font-family: Helvetica; display: block; margin: 0px auto; text-align: center;}
    body{ 
    h1 {color: #262626; margin: 1rem;} 
    h3 {color: #444444; margin-bottom: 10px;}
    text-decoration: none; font-size: 25px; margin-bottom: 10px; cursor: pointer; border-radius: 4px;} 
    .button {display: inline-block; height: 35px; width: 100px; font-size: 1.5rem; background-color: #351b54;
     color: white; padding: 15px 30px; border: 4px solid transparent;}

    .button:hover{border: 4px solid black;} 
    .button-on {background-color: #055214;} 
    .button-off {background-color: #4f1109;}
    p {font-size: 1.5rem; color: #888;margin-bottom: 10px;}
    pre { font-size: 0.8rem}
    </style>

    <h1>ESP32 Web Server</h1>
    <h3>Using Access Point (AP) Mode</h3>
    )rawliteral";

    // Button designs
    ptr += DesignButton(button1status, "button1");
    ptr += DesignButton(button2status, "button2");

    // Other Info in <pre>
    ptr += R"rawliteral(
    <h5>The input element</h5>
    <form action="/inputText">
    <label for="param"> Input: </label>
    <input type="text" id="param" name="param">
    <br><br>
    <input type="submit" value="Submit">
    </form>
    
    <p>Other Information:</p>
    <pre>
    )rawliteral";

    // pre text
    ptr += otherInfo;

    ptr += R"rawliteral(
    </pre>

    </body>
    </html>
    )rawliteral";

    return ptr;
}