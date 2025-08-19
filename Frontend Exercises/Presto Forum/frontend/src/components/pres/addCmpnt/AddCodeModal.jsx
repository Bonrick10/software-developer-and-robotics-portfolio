import React from 'react';
import axios from 'axios';
import { Button, Modal, Form } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function AddCodeModal ({ activeIndex, setSlides, presId }) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const [width, setWidth] = React.useState(0);
  const [height, setHeight] = React.useState(0);
  const [code, setCode] = React.useState('');
  const [lang, setLang] = React.useState('');
  const [fontSize, setFontSize] = React.useState(0);

  const [modalShow, setModalShow] = React.useState(false);
  const handleClose = () => setModalShow(false);
  const handleOpen = () => setModalShow(true);

  const guessLang = () => {
    // TODO: Probably add some patterns to improve guessing
    const langPatterns = {
      c: [
        /#(include.*?>|define.*)/g, // cPreprocessor
        /\b(typedef.*?;|struct.*?(?=\s*?\{))/g, // cStruct
        /\b\w+\s*->\s*\w+\b/g, // cPointers
        /[a-zA-Z]+\s+\*\s+([a-zA-Z_]\w*)\s*\([a-zA-Z]*\s*([a-zA-Z_]\w*)\)\s*\{/g, // cFunctionPointerReturn
        /[a-zA-Z]+\s*\*?\s*([a-zA-Z_]\w*)\(.*[a-zA-Z]+\s*\*\s*([a-zA-Z_]\w*).*\)\s*\{/g // cFunctionPointerArg
      ],
      javascript: [
        /import\s+\*\s+as\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s+from\s+'([^']+)'/, // jsImport
        /class\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*(?:extends\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*)?{/, // jsClassDeclaration
        /function\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*\(([^)]*)\)\s*{/, // jsFunctionDeclaration
        /([a-zA-Z_$][0-9a-zA-Z_$]*)\s*\(([^)]*)\)/, // jsFunctionCall
        /var\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsVarDeclaration
        /let\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsLetDeclaration
        /const\s+([a-zA-Z_$][0-9a-zA-Z_$]*)\s*=\s*([^;]+)/, // jsConstDeclaration
      ],
      python: [
        /(class|def|if|while|else|for).*:/g, // pythonKeywords
        /\[((\d+|("|').+\3),)*(\d+|("|').+\5)\]/g, // pythonLists
        /\bprint(\s*("|').+\2|\(.*\))/g, // pythonPrint
      ],
    }
    const newLang = Object.keys(langPatterns).find(lang =>
      langPatterns[lang].some(pattern =>
        pattern.test(code)));
    setLang(newLang);
  }

  const createCode = async () => {
    if (!width || !height || !code || !fontSize) {
      alert('Please fill empty fields');
    }
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];

      guessLang();
      const newCode = {
        width: width,
        height: height,
        code: code,
        lang: lang,
        fontSize: fontSize,
        zIndex: currSlide.zIndex
      }
      currSlide.zIndex++;
      currSlide.code.push(newCode);
      storeData(userStore);
      setSlides(currPres.slides);
      handleClose();
    } catch (err) {
      console.log(err);
      alert(err.response);
    }
  }

  return (
    <>
      <Button variant="info" onClick={handleOpen}>
        Add Code Block
      </Button>
      <Modal
        show={modalShow}
        onHide={handleClose}
        backdrop="static"
        keyboard={false}
      >
        <Modal.Header closeButton>
          <Modal.Title>Add Code Block</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <Form>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Width</Form.Label>
              <Form.Control
                type="number"
                min="0.001"
                step=".001"
                placeholder="Width %"
                onChange={e => setWidth(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Height</Form.Label>
              <Form.Control
                type="number"
                min="0.001"
                step=".001"
                placeholder="Height %"
                onChange={e => setHeight(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Font Size</Form.Label>
              <Form.Control
                type="font-size"
                placeholder="1(em)"
                onChange={e => setFontSize(e.target.value)}
                autoFocus
              />
            </Form.Group>
            <Form.Group className="mb-3" controlId="exampleForm.ControlInput1">
              <Form.Label>Code Block</Form.Label>
              <Form.Control
                type="textarea"
                onChange={e => setCode(e.target.value)}
                autoFocus
              />
            </Form.Group>
          </Form>
        </Modal.Body>
        <Modal.Footer>
          <Button variant="danger" onClick={handleClose}>
            Cancel
          </Button>
          <Button variant="primary" onClick={createCode}>Create</Button>
        </Modal.Footer>
      </Modal>
    </>
  );
}
