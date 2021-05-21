// Copyright (c) 2021 OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#pragma once

const char tracez_index[] =
    ""
    "<!doctype html>"
    "<html>"
    "  <head>"
    "    <title>zPages TraceZ</title>"
    "    <script src='/tracez/script.js'></script>"
    "    <link href='/tracez/style.css' rel='stylesheet'>"
    "  </head>"
    "  <body>"
    "    <h1>zPages TraceZ</h1>"
    "    <span  id='top-right'>Data last fetched: <span id='lastUpdateTime'></span><br>"
    "    <button onclick='refreshData()'>Fetch new data</button></span>"
    "    <br><br>"
    "    <div class='table-wrap'>"
    "      <table id='headers'>"
    "        <colgroup>"
    "          <col class='md'>"
    "          <col class='sm'>"
    "          <col class='sm'>"
    "          <col class='lg'>"
    "        </colgroup>"
    "        <tr>"
    "          <th>Span Name</th>"
    "          <th>Error Samples</th>"
    "          <th>Running</th>"
    "          <th>Latency Samples</th>"
    "        </tr>"
    "      </table>"
    "      <table id='overview_table'>"
    "      </table>"
    "      <div class='right'>Row count: <span id='overview_table_count'>0</span></div>"
    "    </div>"
    "    <br>"
    "    <br>"
    "    <span id='name_type_detail_table_header'></span>"
    "    <div class='table-wrap'>"
    "      <table id='name_type_detail_table'>"
    "      </table>"
    "      <div class='right'>Row count: <span id='name_type_detail_table_count'>0</span></div>"
    "    </div>"
    "  </body>"
    "</html>";
