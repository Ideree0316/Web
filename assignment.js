document.addEventListener("DOMContentLoaded", () => {
  // Buttons that navigate between pages (may be absent on one of the pages)
  const toCodeExplain = document.getElementById("button1"); // assignment.html: 'Go to Code' button
  const toAssignment = document.getElementById("button2"); // codeexplain.html: 'Back to Assignment' button

  // If present, wire button1 to open the code explanation page
  if (toCodeExplain) {
    toCodeExplain.addEventListener("click", () => {
      // navigate to codeexplain.html when clicked
      window.location.href = "codeexplain.html";
    });
  }

  // If present, wire button2 to return to the assignment page
  if (toAssignment) {
    toAssignment.addEventListener("click", () => {
      // navigate back to assignment.html when clicked
      window.location.href = "index.html";
    });
  }
});
