import React from 'react';
import { CodeBlock, dracula } from 'react-code-blocks';

export function RenderCode ({
  code,
  codeIndex,
  setTargetObject,
  lastClickTime,
  setLastClickTime,
  setMenuPosition,
  setCodeIndex,
  setEditCode,
  setIsCodeContextMenuVisible
}) {
  const handleCodeContextMenu = (event) => {
    event.preventDefault();
    setIsCodeContextMenuVisible(true);
    setCodeIndex(codeIndex);
    setMenuPosition({ x: event.clientX * 1.2, y: event.clientY * 1.2 });
  };

  const handleCodeDoubleClick = () => {
    setTargetObject(code);
    const now = Date.now();
    if (now - lastClickTime < 500) {
      setCodeIndex(codeIndex);
      setEditCode(true);
    } else {
      setLastClickTime(now);
      setEditCode(false);
    }
  };

  return (
    <div
      key={codeIndex}
      className="codeBox"
      onContextMenu={(e) =>
        handleCodeContextMenu(e)
      }
      onClick={() =>
        handleCodeDoubleClick()
      }
      style={{
        width: `${code.width}%`,
        height: `${code.height}%`,
        fontSize: `${code.fontSize}em`,
        fontFamily: 'courier',
        zIndex: `${code.zIndex}`
      }}>
      <CodeBlock
        text={code.code}
        language={code.lang}
        showLineNumbers={true}
        theme={dracula}
        style={{
          width: '100%',
          height: '100%'
        }}
      />
    </div>
  )
}
