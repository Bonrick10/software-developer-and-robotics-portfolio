import React from 'react';
import { Navbar, Nav } from 'react-bootstrap';

export function AuthNavBar () {
  return (
    <Navbar className="bg-body-tertiary justify-content-between">
      <Navbar.Brand>Presto</Navbar.Brand>
      <Nav fill variant="tabs">
        <Nav.Item>
          <Nav.Link href='/login'>Login</Nav.Link>
        </Nav.Item>
        <Nav.Item>
          <Nav.Link href='/register'>Register</Nav.Link>
        </Nav.Item>
      </Nav>
    </Navbar>
  )
}
