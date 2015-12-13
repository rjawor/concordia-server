function searchHandle(tmid) {
    var concordiaRequest = {
        operation: 'concordiaSearch',
        tmId: tmid,
        pattern:$("#searchInput").val()
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

function renderResult(data) {
    var res = '';
    
    var score = data['result']['bestOverlayScore']*100;
    
    res += '<div id="result-score">Concordia score: <b>'+score.toFixed(0)+'%</b></div>';
    
    var inputSentence = $('#searchInput').val();
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
    
    res += '<div id="result-sentence">'+markedSentence+'</div>';
    
    res += '<br/><br/><br/>'+fragments;
    
    return res;
}

function renderFragment(fragment, number) {
    var result = '<div style="display:none" id="fragment'+number+'" class="fragmentDetails"><table><tr><td>';

    var sourceSegment = fragment['sourceSegment'];
    result += sourceSegment.slice(0, fragment['matchedExampleStart']);
    result += '<span class="matchedFragment">';
    result += sourceSegment.slice(fragment['matchedExampleStart'], fragment['matchedExampleEnd']);
    result += '</span>';
    result += sourceSegment.slice(fragment['matchedExampleEnd']);
    
    result += '</td></tr><tr><td>'+fragment['targetSegment']+'</td></tr></table></div>';
    return result;
}

function displayDetails(caller, number) {
    $('#result-sentence .matchedFragmentSelected').attr("class","matchedFragment");
    caller.className='matchedFragmentSelected';
    $('.fragmentDetails').css('display', 'none');
    $('#fragment'+number).css('display', 'block');
}

function searchText(text, tmid) {
    $("#searchInput").val(text);
    searchHandle(tmid);
}

function showHideSuggestions() {
    $('#suggestions').toggleClass('suggestionsInvisible');
}
