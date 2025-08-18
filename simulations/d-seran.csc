<?xml version="1.0" encoding="UTF-8"?>
<simconf version="2023090101">
  <simulation>
    <title>D-SERAN test moderne</title>
    <randomseed>123456</randomseed>
    <motedelay_us>1000000</motedelay_us>
    <radiomedium>
      org.contikios.cooja.radiomediums.UDGM
      <transmitting_range>50.0</transmitting_range>
      <interference_range>100.0</interference_range>
      <success_ratio_tx>1.0</success_ratio_tx>
      <success_ratio_rx>1.0</success_ratio_rx>
    </radiomedium>
  <events>
      <logoutput>60000</logoutput>
    <event>
        <time>60000</time>
      <command>quit</command>
    </event>
  </events>
    <motetype>
      org.contikios.cooja.contikimote.ContikiMoteType
      <identifier>d-seran</identifier>
      <description>D-SERAN Mote</description>
      <source>[CONFIG_DIR]/../src/d-seran.c</source>
      <commands>$(MAKE) -j$(CPUS) d-seran.cooja TARGET=cooja</commands>
      <firmware>[CONFIG_DIR]/../src/build/cooja/d-seran.cooja</firmware>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="50" y="50" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="100" y="50" />
        </interface_config>
      </mote>
      <mote>
        <interface_config>
          org.contikios.cooja.interfaces.Position
          <pos x="75" y="100" />
        </interface_config>
      </mote>
    </motetype>
  </simulation>
  <plugin>
    org.contikios.cooja.plugins.LogListener
    <plugin_config>
      <filter />
      <formatted_time />
      <coloring />
    </plugin_config>
    <bounds x="400" y="160" height="240" width="100" z="2" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.TimeLine
    <plugin_config>
      <mote>0</mote>
      <showRadioRXTX />
      <showRadioHW />
      <showLEDs />
      <zoomfactor>500.0</zoomfactor>
    </plugin_config>
    <bounds x="0" y="442" height="166" width="500" z="1" />
  </plugin>
  <plugin>
    org.contikios.cooja.plugins.ScriptRunner
    <plugin_config>
      <script>
        /* Ecrire chaque message de mote dans le test log */
        while (true) {
          YIELD();
          if (typeof msg !== 'undefined' &amp;&amp; msg !== null) {
            log.log(String(msg) + "\n");
          }
        }
      </script>
      <active>true</active>
    </plugin_config>
  </plugin>
</simconf> 