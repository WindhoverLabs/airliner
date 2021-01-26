Limit Checker
====================================

The :term:`LC` CFS Limit Checker application monitors telemetry data points in the flight system and compares the values against predefined threshold limits. When a threshold condition is encountered, an event message is issued and a Relative Time Sequence (RTS) command script may be initiated to respond to the threshold violation.

LC is a table driven application. Monitoring and response configuration data is stored in definition tables that can be updated by the ground operators, while statistics on threshold violations are stored in dump-only results tables and summarized in housekeeping data.

When processing beyond the standard comparison operators is needed, LC supports the use of custom evaluation functions that can be modified by a software patch or application update.

.. toctree::
   :caption: Contents
   :name: lc_contents
   
   Issues and Limitations <docs/limitations>
   Design <docs/design>
   Activities <docs/activities>
   Configuration <docs/configuration>
   Concept of Operations <docs/conops>
   FAQ <docs/faq>


Detailed design documentation is available `here <../../doxy/apps/lc/index.html>`_.

.. image:: /docs/_static/doxygen.png
   :target: ../../doxy/apps/lc/index.html