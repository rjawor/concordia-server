function toggleHelp() {
    $('#help').toggleClass('hidden');
}

function searchHandle(tmid) {
    var concordiaRequest = {
        operation: 'concordiaSearch',
        tmId: tmid,
        pattern:$("#search-input").val()
    }

    $.ajax({
        url: '/concordia_gate.php',
        type: 'post',
        dataType: 'json',
        success: function (data) {
            $('#result').html(renderResult(data));
        },
        data: concordiaRequest
    });
}

function phraseSearchHandle(tmid, intervals) {
    var concordiaRequest = {
        operation: 'concordiaPhraseSearch',
        tmId: tmid,
        pattern:$("#search-input").val(),
        intervals: intervals
    }


    $.ajax({
        url: '/concordia_gate.php',
        type: 'post',
        dataType: 'json',
        success: function (data) {
            if (data['found']) {
                $('#result').html(renderResult(data));
            } else {
               $('#phrase-prompt').html('<b>Your phrase was not found. Try selecting another phrase: </b>').fadeOut(200).fadeIn(200);
               clearTextSelections();
            }
        },
        data: concordiaRequest
    });
}

function renderResult(data) {
    var res = '';
    
    if (typeof(data['result']['bestOverlayScore']) === 'undefined') {
        // ignore    
    } else {
        var score = data['result']['bestOverlayScore']*100;
        res += '<div id="result-score">Concordia score: <b>'+score.toFixed(0)+'%</b></div>';
    }
    res += '<div id="phrase-selection">Phrase search mode:&nbsp;<img id="phrase-off-icon" src="../images/switchOff.png" alt="enter phrase search mode" onclick="togglePhraseSearchMode()" title="search for phrases"/><img class="hidden" id="phrase-on-icon" src="../images/switchOn.png" alt="cancel phrase search" onclick="togglePhraseSearchMode()" title="cancel phrase search"/><span id="phrase-prompt" class="hidden">Select continuous phrase: </span></div>';
    
    var inputSentence = $('#search-input').val();
    var markedSentence = '';
    var fragments = '';
    lastInsertedEnd = 0;
    for(var i = 0; i < data['result']['bestOverlay'].length; i++) {
        var fragment = data['result']['bestOverlay'][i];
        //previous unmarked fragment
        markedSentence += inputSentence.slice(lastInsertedEnd, fragment['matchedPatternStart']);

        //the marked fragment
        markedSentence += '<span onclick="displayDetails(this, '+i+')" class="matchedFragment">'+inputSentence.slice(fragment['matchedPatternStart'], fragment['matchedPatternEnd'])+'</span>';
        
        lastInsertedEnd = fragment['matchedPatternEnd'];
        
        fragments += renderFragment(fragment, i);
    }
    
    //remaining unmarked fragment
    markedSentence += inputSentence.slice(lastInsertedEnd);
    
    res += '<div id="result-sentence" onMouseUp="phraseSearch(this)">'+markedSentence+'</div>';
    
    res += '<br/><br/><br/>'+fragments;
    
    return res;
}

function renderFragment(fragment, number) {
    var result = '<div style="display:none" id="fragment'+number+'" class="fragmentDetails"><table><tr><td>';

    // source segment
    var sourceSegment = fragment['sourceSegment'];
    result += sourceSegment.slice(0, fragment['matchedExampleStart']);
    result += '<span class="matchedFragment">';
    result += sourceSegment.slice(fragment['matchedExampleStart'], fragment['matchedExampleEnd']);
    result += '</span>';
    result += sourceSegment.slice(fragment['matchedExampleEnd']);
    
    // target segment
    result += '</td></tr><tr><td>';
    var targetSegment = fragment['targetSegment'];
    var currStart = 0;
    for (i=0;i<fragment['targetFragments'].length;i++) {
        result += targetSegment.slice(currStart, fragment['targetFragments'][i][0]);
        result += '<span class="matchedFragment">';
        result += targetSegment.slice(fragment['targetFragments'][i][0], fragment['targetFragments'][i][1]);
        result += '</span>';
        currStart = fragment['targetFragments'][i][1];
    }
    result += targetSegment.slice(currStart);
    result += '</td></tr></table></div>';
    return result;
}

function togglePhraseSearchMode() {
    $('#result-sentence').toggleClass('phrase-mode');
    $('#phrase-on-icon').toggleClass('hidden');
    $('#phrase-off-icon').toggleClass('hidden');
    $('#phrase-prompt').toggleClass('hidden');
    clearTextSelections();
}

function displayDetails(caller, number) {
    if (!$('#result-sentence').hasClass('phrase-mode')) {
        $('#result-sentence .matchedFragmentSelected').attr("class","matchedFragment");
        caller.className='matchedFragmentSelected';
        $('.fragmentDetails').css('display', 'none');
        $('#fragment'+number).css('display', 'block');
    }
}

function searchText(text, tmid) {
    $("#search-input").val(text);
    searchHandle(tmid);
}

function showHideSuggestions() {
    $('#suggestions').toggleClass('suggestionsInvisible');
}

function phraseSearch(caller) {
    if ($('#result-sentence').hasClass('phrase-mode')) {
        var phrase = getSelectedTextWithin(caller);
        if (phrase.length > 0) {
        
            var intervalStarts = getIndicesOf(phrase, $("#search-input").val(), true);
            var intervals = [];
            
            for (var i=0;i<intervalStarts.length;i++) {
                
                intervals.push([intervalStarts[i], intervalStarts[i]+phrase.length])
            }
            
            phraseSearchHandle(currentTmId, intervals);
        }
    }
}

function getSelectedTextWithin(el) {
    var selectedText = "";
    if (typeof window.getSelection != "undefined") {
        var sel = window.getSelection(), rangeCount;
        if ( (rangeCount = sel.rangeCount) > 0 ) {
            var range = document.createRange();
            for (var i = 0, selRange; i < rangeCount; ++i) {
                range.selectNodeContents(el);
                selRange = sel.getRangeAt(i);
                if (selRange.compareBoundaryPoints(range.START_TO_END, range) == 1 && selRange.compareBoundaryPoints(range.END_TO_START, range) == -1) {
                    if (selRange.compareBoundaryPoints(range.START_TO_START, range) == 1) {
                        range.setStart(selRange.startContainer, selRange.startOffset);
                    }
                    if (selRange.compareBoundaryPoints(range.END_TO_END, range) == -1) {
                        range.setEnd(selRange.endContainer, selRange.endOffset);
                    }
                    selectedText += range.toString();
                }
            }
        }
    } else if (typeof document.selection != "undefined" && document.selection.type == "Text") {
        var selTextRange = document.selection.createRange();
        var textRange = selTextRange.duplicate();
        textRange.moveToElementText(el);
        if (selTextRange.compareEndPoints("EndToStart", textRange) == 1 && selTextRange.compareEndPoints("StartToEnd", textRange) == -1) {
            if (selTextRange.compareEndPoints("StartToStart", textRange) == 1) {
                textRange.setEndPoint("StartToStart", selTextRange);
            }
            if (selTextRange.compareEndPoints("EndToEnd", textRange) == -1) {
                textRange.setEndPoint("EndToEnd", selTextRange);
            }
            selectedText = textRange.text;
        }
    }
    return selectedText;
}

function clearTextSelections() {
    if (window.getSelection) {
      if (window.getSelection().empty) {  // Chrome
        window.getSelection().empty();
      } else if (window.getSelection().removeAllRanges) {  // Firefox
        window.getSelection().removeAllRanges();
      }
    } else if (document.selection) {  // IE?
      document.selection.empty();
    }
}

function getIndicesOf(searchStr, str, caseSensitive) {
    var startIndex = 0, searchStrLen = searchStr.length;
    var index, indices = [];
    if (!caseSensitive) {
        str = str.toLowerCase();
        searchStr = searchStr.toLowerCase();
    }
    while ((index = str.indexOf(searchStr, startIndex)) > -1) {
        indices.push(index);
        startIndex = index + searchStrLen;
    }
    return indices;
}
