//
// The contents of this file are subject to the Mozilla Public
// License Version 1.1 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy
// of the License at http://www.mozilla.org/MPL/
//
// Software distributed under the License is distributed on an
// "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
// implied. See the License for the specific language governing
// rights and limitations under the License.
//
// The Original Code is State Machine Compiler (SMC).
//
// The Initial Developer of the Original Code is Charles W. Rapp.
// Portions created by Charles W. Rapp are
// Copyright (C) 2000 - 2007. Charles W. Rapp.
// All Rights Reserved.
//
// Contributor(s):
//      Port to JavaScript by Francois Perrad, francois.perrad@gadz.org
//
// Name
//  Telephone.js
//
// Description
//  A simulation of an old fashioned touch-tone telephone.
//
// RCS ID
// $Id$
//
// CHANGE LOG
// $Log$
// Revision 1.1  2013/12/20 18:58:39  fperrad
// full refactor of JavaScript
//
//
//

function Telephone () {
    this.areaCode = '';
    this.exchange = '';
    this.local = '';
    this.callType = 0;
    this._timerMap = {};
    this._delayMap = {};
    this._timerAudio = null;
    this._sound = null;
    this._clip = null;

    this._fsm = new TelephoneContext(this);
    this._fsm.setDebugStream(new Debug());
    this._fsm.setDebugFlag(true);
}

Telephone.LONG_DISTANCE = 1;
Telephone.LOCAL = 2;
Telephone.EMERGENCY = 3;
Telephone.NYC_TEMP = 4;
Telephone.TIME = 5;
Telephone.DEPOSIT_MONEY = 6;
Telephone.LINE_BUSY = 7;
Telephone.INVALID_NUMBER = 8;
Telephone.SEC_PER_MINUTE = 60;

Telephone.prototype.Start = function () {
    this._fsm.debugStream.clear();
    this._fsm.enterStartState();
};

Telephone.prototype.Digit = function (n) {
    this._fsm.Digit(n);
};

Telephone.prototype.Receiver = function () {
    var text = document.getElementById('receiver').value;
    if (text == 'Pick up receiver') {
        this._fsm.OffHook();
    }
    else if (text == 'Put down receiver') {
        this._fsm.OnHook();
    }
    else {
        alert('Unknown receiver ('+text+')');
    }
};

/*-----------------------------------------------------------
 * State Machine Actions.
 */

Telephone.prototype.routeCall = function (callType, areaCode, exchange, local) {
    var route;
    if (callType == Telephone.EMERGENCY) {
        route = Telephone.EMERGENCY;
    }
    else if (callType == Telephone.LONG_DISTANCE
          && areaCode == '1212'
          && exchange == '555'
          && local == '1234') {
        route = Telephone.NYC_TEMP;
    }
    else if (exchange == '555') {
        if (local == '1212') {
            route = Telephone.TIME;
        }
        else {
            route = Telephone.LINE_BUSY;
        }
    }
    else if (callType == Telephone.LOCAL) {
        route = Telephone.DEPOSIT_MONEY;
    }
    else {
        route = Telephone.INVALID_NUMBER;
    }

    // Call routing needs to be done asynchronouzly in order to
    // avoid issuing a transition within a transition.
    setTimeout('app._callRoute('+route+');', 50);
};

Telephone.prototype._callRoute = function (route) {
    switch (route) {
        case Telephone.EMERGENCY:
            app._fsm.Emergency();
             break;
        case Telephone.NYC_TEMP:
            app._fsm.NYCTemp();
            break;
        case Telephone.TIME:
            app._fsm.Time();
            break;
        case Telephone.DEPOSIT_MONEY:
            app._fsm.DepositMoney();
            break;
        case Telephone.LINE_BUSY:
            app._fsm.LineBusy();
            break;
        case Telephone.INVALID_NUMBER:
            app._fsm.InvalidNumber();
            break;
    }
};

Telephone.prototype.startTimer = function (name, delay) {
    this._delayMap[name] = delay;
    this._timerMap[name] = setTimeout('app._fsm.'+name+'();', delay);
};

Telephone.prototype.resetTimer = function (name) {
    if (this._timerMap[name] != null) {
        clearTimeout(this._timerMap[name]);
        this._timerMap[name] = setTimeout('app._fsm.'+name+'();', this._delayMap[name]);
    }
};

Telephone.prototype.stopTimer = function (name) {
    if (this._timerMap[name] != null) {
        clearTimeout(this._timerMap[name]);
        this._timerMap[name] = null;
    }
};

Telephone.prototype.play = function (name) {
    var sound = document.getElementById('audio_' + name);
    sound.loop = false;
    sound.play();
    this._sound = sound;
    if (this._timerAudio != null) {
        clearTimeout(this._timerAudio);
    }
    this._timerAudio = setTimeout('app._fsm.PlaybackDone();', 1000 * sound.duration);
};

Telephone.prototype.playTT = function (name) {
    var sound = document.getElementById('audio_touch_tone_' + name);
    sound.loop = false;
    sound.play();
};

Telephone.prototype.loop = function (name) {
    var sound = document.getElementById('audio_' + name);
    sound.loop = true;
    sound.play();
    this._sound = sound;
};

Telephone.prototype.stopLoop = function (name) {
    this._sound.pause();
    this._sound = null;
};

Telephone.prototype.stopPlayback = function () {
    if (this._timerAudio != null) {
        clearTimeout(this._timerAudio);
        this._timerAudio = null;
    }
    if (this._sound != null) {
        this._sound.pause();
        this._sound = null;
    }
};

Telephone.prototype.playEmergency = function () {
    this.play('911');
};

Telephone.prototype.playNYCTemp = function () {
    this.play('nyctemp');
};

Telephone.prototype.playDepositMoney = function () {
    this.play('50_cents_please');
};

Telephone.prototype.playTime = function () {
    this._clip = new Array();
    this._clip.push('the_time_is');
    var date = new Date();
    var pm = false;
    var hours = date.getHours();
    if (hours > 12) {
        pm = true;
        hours -= 12;
    }
    var minutes = date.getMinutes();
    var seconds = date.getSeconds();
    // 1. Read the hour.
    this._clip.push(hours.toString());
    // Is this on the hour exactly?
    if (minutes == 0 && seconds == 0) {
        this._clip.push('oclock');
        this._clip.push(pm ? 'PM' : 'AM');
        this._clip.push('exactly');
    }
    else {
        // 2. Read the minute.
        this._soundNumber(minutes, true);
        this._clip.push(pm ? 'PM' : 'AM');
        // 3. Read the seconds.
        if (seconds == 0) {
            this._clip.push('exactly');
        }
        else {
            this._clip.push('and');
            this._soundNumber(seconds, false);
            this._clip.push(seconds == 1 ? 'second' : 'seconds');
        }
    }
    this.playClip();
};

Telephone.prototype._soundNumber = function (number, zeroFlag) {
    if (number < 10 && zeroFlag == true) {
        this._clip.push('oh');
        this._clip.push(number.toString());
    }
    else if (number < 20) {
        this._clip.push(number.toString());
    }
    else {
        var onesDigit = number % 10;
        var tensDigit = number - onesDigit;
        // Read the ten's digit first and then the
        // remainder - if not zero.
        this._clip.push(tensDigit.toString());
        if (onesDigit != 0) {
            this._clip.push(onesDigit.toString());
        }
    }
};

Telephone.prototype.playInvalidNumber = function () {
    this._clip = new Array();
    this._clip.push('the_number_you_have_dialed');
    // If this is a long distance number, sound out the
    // area code first.
    if (this.callType == Telephone.LONG_DISTANCE) {
        for (var i = 0; i < this.areaCode.length; ++i) {
            this._clip.push(this.areaCode.substr(i, 1));
        }
    }
    // All types have an exchange.
    for (var i = 0; i < this.exchange.length; ++i) {
        this._clip.push(this.exchange.substr(i, 1));
    }
    // Only long distance and local numbers have a local
    // portion.
    if (this.callType == Telephone.LONG_DISTANCE
     || this.callType == Telephone.LOCAL) {
        for (var i = 0; i < this.local.length; ++i) {
            this._clip.push(this.local.substr(i, 1));
        }
    }
    this._clip.push('could_not_be_completed');
    this.playClip();
};

Telephone.prototype.playClip = function () {
    var sound = document.getElementById('audio_' + this._clip.shift());
    sound.loop = false;
    sound.play();
    this._sound = sound;
    if (this._timerAudio != null) {
        clearTimeout(this._timerAudio);
    }
    if (this._clip.length == 0) {
        this._timerAudio = setTimeout('app._fsm.PlaybackDone();', 1000 * sound.duration);
    }
    else {
        this._timerAudio = setTimeout('app.playClip();', 1000 * sound.duration);
    }
};

Telephone.prototype.saveAreaCode = function (n) {
    this.areaCode += n;
    this.addDisplay(n);
};

Telephone.prototype.saveExchange = function (n) {
    this.exchange += n;
    this.addDisplay(n);
};

Telephone.prototype.saveLocal = function (n) {
    this.local += n;
    this.addDisplay(n);
};

Telephone.prototype.addDisplay = function (character) {
    document.getElementById('display').value += character;
};

Telephone.prototype.clearDisplay = function () {
    document.getElementById('display').value = '';
    // Clear the internal data store.
    this.areaCode = '';
    this.local = '';
    this.exchange = '';
};

Telephone.prototype.startClockTimer = function () {
    var date = new Date();
    var timeRemaining = Telephone.SEC_PER_MINUTE - date.getSeconds();
    app.startTimer('ClockTimer', timeRemaining * 1000);
};

Telephone.prototype.updateClock = function () {
    var months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec'];
    var date = new Date();
    var hours = date.getHours().toString();
    if (hours.length == 1) {
        hours = '0' + hours;
    }
    var minutes = date.getMinutes().toString()
    if (minutes.length == 1) {
        minutes = '0' + minutes;
    }
    document.getElementById('display').value = hours
                                             + ':'
                                             + minutes
                                             + '  '
                                             + months[date.getMonth()]
                                             + ' '
                                             + date.getDate()
                                             + ', '
                                             + date.getFullYear();
};

Telephone.prototype.setReceiver = function (text) {
    document.getElementById('receiver').value = text;
};

function Debug () {}
Debug.prototype.clear = function () {
    document.getElementById('trace').value = '';
};
Debug.prototype.write = function (s) {
    document.getElementById('trace').value += s;
};

var app = new Telephone();
app.Start();
