// Shortcut for document.getElementById
function $(id) {
  return document.getElementById(id);
}

// ---------- Showing the data the server sends back ----------

function showMessage(msg, isError) {
  var box = $("message");
  box.textContent = msg;
  box.className = "message";
  if (msg === "") {
    return;
  }
  box.classList.add(isError ? "error" : "ok");
}

// Server replies that start with these words mean something went wrong
function looksLikeError(msg) {
  return (
    msg.indexOf("Error") === 0 ||
    msg.indexOf("Usage") === 0 ||
    msg.indexOf("Unknown") === 0 ||
    msg.indexOf("Nothing to") === 0
  );
}

function showHistory(historyText) {
  var list = $("history");
  list.innerHTML = "";

  var lines = historyText.split("\n").filter(function (l) {
    return l.trim() !== "";
  });

  if (lines.length === 0 || historyText === "History is empty.") {
    var empty = document.createElement("li");
    empty.className = "empty";
    empty.textContent = "No operations yet.";
    list.appendChild(empty);
    return;
  }

  lines.forEach(function (line) {
    var li = document.createElement("li");
    var isCurrent = line.indexOf("<-- current") !== -1;
    var isUndone = line.indexOf("(undone)") !== -1;

    // remove the markers, we show them our own way
    var clean = line.replace("<-- current", "").replace("(undone)", "").trim();
    li.textContent = clean;

    if (isCurrent) {
      li.classList.add("current");
      var badge = document.createElement("span");
      badge.className = "badge";
      badge.textContent = "current";
      li.appendChild(badge);
    }
    if (isUndone) {
      li.classList.add("undone");
    }
    list.appendChild(li);
  });
}

function showPill(id, isOn) {
  var pill = $(id);
  pill.textContent = isOn ? "has items" : "empty";
  pill.className = "pill " + (isOn ? "on" : "off");
}

function render(data) {
  // document
  var doc = $("text");
  if (data.text === "") {
    doc.textContent = "(the document is empty)";
    doc.classList.add("empty");
  } else {
    doc.textContent = data.text;
    doc.classList.remove("empty");
  }

  // message
  showMessage(data.message, looksLikeError(data.message));

  // history panel
  showHistory(data.history);

  // buttons and stack status
  $("undoBtn").disabled = !data.canUndo;
  $("redoBtn").disabled = !data.canRedo;
  showPill("undoStatus", data.canUndo);
  showPill("redoStatus", data.canRedo);
}

// ---------- Talking to the C++ server ----------

function send(command) {
  fetch("/api/command", { method: "POST", body: command })
    .then(function (res) {
      return res.json();
    })
    .then(render)
    .catch(function () {
      showMessage("Cannot reach the server. Is server.exe running?", true);
    });
}

function loadState() {
  fetch("/api/state")
    .then(function (res) {
      return res.json();
    })
    .then(render)
    .catch(function () {
      showMessage("Cannot reach the server. Is server.exe running?", true);
    });
}

// ---------- Buttons and keyboard ----------

function runFromBox() {
  var box = $("cmd");
  var command = box.value.trim();
  if (command === "") {
    return;
  }
  send(command);
  box.value = "";
  box.focus();
}

$("runBtn").addEventListener("click", runFromBox);

$("cmd").addEventListener("keydown", function (e) {
  if (e.key === "Enter") {
    runFromBox();
  }
});

$("undoBtn").addEventListener("click", function () {
  send("UNDO");
});

$("redoBtn").addEventListener("click", function () {
  send("REDO");
});

// New line button
$("newlineBtn").addEventListener("click", function () {
  send("NEWLINE");
  $("cmd").focus();
});

// Quick-fill chips: the ones marked data-run run right away,
// the others fill the box with a template for you to complete
var chips = document.querySelectorAll(".chip");
chips.forEach(function (chip) {
  chip.addEventListener("click", function () {
    var cmd = chip.getAttribute("data-cmd");
    if (chip.getAttribute("data-run")) {
      send(cmd);
    } else {
      var box = $("cmd");
      box.value = cmd;
      box.focus();
    }
  });
});

// First load
loadState();